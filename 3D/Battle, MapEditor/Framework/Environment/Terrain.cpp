#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader * shader, wstring heightFile)
	: Renderer(shader)
{
	heightMap = new Texture(heightFile);

	sBaseMap = shader->AsSRV("BaseMap");
	sLayerMap = shader->AsSRV("LayerMap");
	sAlphaMap = shader->AsSRV("AlphaMap");


	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_Brush");

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	sLineBuffer = shader->AsConstantBuffer("CB_TerrainLine");


	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
	CreateTriangleData();

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(TerrainVertex), 0, true);
	indexBuffer = new IndexBuffer(indices, indexCount);

	csShader = new Shader(L"29_Intersection.fxo");

	size = 1 * (indexCount / 3);
	csbuffer = new StructuredBuffer
	(
		input,
		sizeof(InputDesc), size,
		sizeof(OutputDesc), size
	);
	output = new OutputDesc[size];
}

Terrain::~Terrain()
{
	SafeDelete(brushBuffer);
	SafeDelete(lineBuffer);
	SafeDelete(heightMap);
	
	SafeDelete(baseMap);
	SafeDelete(layerMap);
	SafeDelete(alphaMap);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

void Terrain::Update()
{
	Super::Update();

	//Brush
	{
		static bool brushOn = false;
		ImGui::Checkbox("Brush", &brushOn);
		if (brushOn == true)
		{
			ImGui::Begin("BrushEditor");
			{
				static float heightPower = 5.0f;

				ImGui::InputInt("BrushType", (int *)&brushDesc.Type);
				brushDesc.Type %= 3;

				ImGui::InputInt("HeightType", (int *)&brushDesc.HeightType);
				brushDesc.HeightType %= 4;

				if (heightPower <= 20.0f)
					ImGui::InputFloat("HeightPower", &heightPower, 0.1f);
				if (heightPower > 20.0f)
					heightPower = 20.0f;
				if (heightPower < 0.0f)
					heightPower = 0.0f;

				ImGui::InputInt("Range", (int *)&brushDesc.Range);
				brushDesc.Range %= 21;

				if(brushDesc.HeightType == 3)
				ImGui::InputInt("Range2", (int *)&brushDesc.Range2);
				brushDesc.Range2 %= 21;

				if (brushDesc.Type > 0)
				{
					static bool clickOn = false;
					ImGui::Checkbox("== ClickOn/Off : P ==", &clickOn);
					if (Keyboard::Get()->Down('P'))
					{
						if (clickOn == true)
							clickOn = false;
						else if (clickOn == false)
							clickOn = true;
					}
					//마우스의 위치
					brushDesc.Location = GetPickedPosition();

					if (brushDesc.Type == 1)
						ImGui::Text("== Square ==");
					else if (brushDesc.Type == 2)
						ImGui::Text("== Circle ==");

					switch (brushDesc.HeightType)
					{
					case 0:	//기본 올리기
						ImGui::Text("== Height ==");
						ImGui::Text("== 'Shift' : Up/Down ==");

						if (Mouse::Get()->Press(0) && clickOn == true)
							RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.HeightType, brushDesc.Range, heightPower);
						break;

					case 1:	//스무스하게 올리기
						ImGui::Text("== SmoothHeight ==");
						ImGui::Text("== 'Shift' : Up/Down ==");

						if (Mouse::Get()->Press(0) && clickOn == true)
							RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.HeightType, brushDesc.Range, heightPower);
						break;

					case 2:	//평탄화
						ImGui::Text("== Flatten ==");
						ImGui::Text("== 'Shift' : Curr/Power ==");

						if (Mouse::Get()->Press(0) && clickOn == true)
							MiddleHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range, fixed, heightPower);
						else
							fixed = false;
						break;

					case 3:		//경사로
						ImGui::Text("== Slope ==");
						ImGui::Text("== 'Shift' : Up/Down ==");
						ImGui::Text("== Range : X, Range2 : Y ==");
						ImGui::Text("== Input Direction ==");

						//방향 입력
						{
							if (Keyboard::Get()->Down(VK_LEFT))
								brushDesc.SlopeDirection = 0;
							if (Keyboard::Get()->Down(VK_RIGHT))
								brushDesc.SlopeDirection = 1;
							if (Keyboard::Get()->Down(VK_UP))
								brushDesc.SlopeDirection = 2;
							if (Keyboard::Get()->Down(VK_DOWN))
								brushDesc.SlopeDirection = 3;
						}

						if (Mouse::Get()->Press(0) && clickOn == true)
							SlopeHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range, brushDesc.Range2, brushDesc.SlopeDirection, heightPower);
						//ImGui::SliderFloat("XLength", &power, 1, 20);
						break;
					}

				}
			}
			ImGui::End();
		}
	}

	//Line
	{
		/*static bool lineOn = false;
		ImGui::Checkbox("Line", &lineOn);
		if (lineOn == true)
		{
			ImGui::Begin("LineEditor");
			{
				ImGui::ColorEdit3("Color", lineDesc.Color);

				ImGui::InputInt("Visible", (int *)&lineDesc.Visible);
				lineDesc.Visible %= 2;

				ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.001f);
				lineDesc.Thickness = Math::Clamp(lineDesc.Thickness, 0.01f, 0.9f);

				ImGui::InputFloat("Size", &lineDesc.Size, 1.0f);
			}
			ImGui::End();
		}*/
	}
	
}

void Terrain::Render()
{
	Super::Render();

	if (baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());

	if (layerMap != NULL)
	{
		sLayerMap->SetResource(layerMap->SRV());
		sAlphaMap->SetResource(alphaMap->SRV());
	}


	brushBuffer->Apply();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());

	lineBuffer->Apply();
	sLineBuffer->SetConstantBuffer(lineBuffer->Buffer());

	shader->DrawIndexed(0, Pass(), indexCount);
}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void Terrain::LayerMap(wstring file, wstring alphaFile)
{
	SafeDelete(layerMap);
	SafeDelete(alphaMap);

	layerMap = new Texture(file);
	alphaMap = new Texture(alphaFile);
}

float Terrain::GetHeight(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;


	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;

	
	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	Vector3 result;

	if (ddx + ddz <= 1.0f)
	{
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	}
	else
	{
		ddx = 1.0f - ddx;
		ddz = 1.0f - ddz;

		result = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}


	return result.y;
}

float Terrain::GetHeightPick(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;


	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;


	Vector3 start(position.x, 1000, position.z);
	Vector3 direction(0, -1, 0);

	float u, v, distance;
	Vector3 result(-1, FLT_MIN, -1);

	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

Vector3 Terrain::GetPickedPosition()
{
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* Vp = Context::Get()->GetViewport();

	Vector3 mouse = Mouse::Get()->GetPosition();


	Vector3 n, f;
	mouse.z = 0.0f;
	Vp->Unproject(&n, mouse, transform->World(), V, P);

	mouse.z = 1.0f;
	Vp->Unproject(&f, mouse, transform->World(), V, P);

	Vector3 start = n;
	Vector3 direction = f - n;

	//////////////////////////////////////////////////////////////////////

	ray.Position = start;
	ray.Direction = direction;

	csShader->AsVector("Position")->SetFloatVector(ray.Position);
	csShader->AsVector("Direction")->SetFloatVector(ray.Direction);
	csShader->AsSRV("Input")->SetResource(csbuffer->SRV());
	csShader->AsUAV("Output")->SetUnorderedAccessView(csbuffer->UAV());

	csShader->Dispatch(0, 0, 140, 1, 1);

	csbuffer->Copy(output, sizeof(OutputDesc) * size);

	for (UINT i = 0; i < indexCount / 3; i++)
	{
		if (output[i].Picked == 1)
		{
			return Vector3(input[i].V0 + (input[i].V1 - input[i].V0) + (input[i].V2 - input[i].V0));
		}
	}

	return Vector3(-5, -5, -5);	//고정된 노피킹값
}

void Terrain::RaiseHeight(Vector3 & position, UINT type, UINT type2, UINT range, float power)
{
	D3D11_RECT rect;
	rect.left = (LONG)position.x - range;
	rect.top = (LONG)position.z + range;
	rect.right = (LONG)position.x + range;
	rect.bottom = (LONG)position.z - range;


	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width) rect.right = (LONG)width;
	if (rect.bottom < 0) rect.bottom = 0;

	if (type == 1)
	{
		for (LONG z = rect.bottom; z <= rect.top; z++)
		{
			for (LONG x = rect.left; x <= rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				if (type2 == 0) {
					if (Keyboard::Get()->Press(VK_LSHIFT))
						vertices[index].Position.y -= power * Time::Delta();
					else
						vertices[index].Position.y += power * Time::Delta();
				}
				if (type2 == 1)
				{
					float dx = vertices[index].Position.x - (LONG)position.x;
					float dz = vertices[index].Position.z - (LONG)position.z;
					float dist = sqrt(dx * dx + dz * dz);

					float perdist = dist / range;

					if (Keyboard::Get()->Press(VK_LSHIFT))
						vertices[index].Position.y -= cos(perdist) * power * Time::Delta();
					else
						vertices[index].Position.y += cos(perdist) * power * Time::Delta();
				}
				
			}
		}
	}
	if (type == 2)
	{
		for (LONG z = rect.bottom; z <= rect.top; z++)
		{
			for (LONG x = rect.left; x <= rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				float dx = vertices[index].Position.x - (LONG)position.x;
				float dz = vertices[index].Position.z - (LONG)position.z;
				float dist = sqrt(dx * dx + dz * dz);

				if (type2 == 0) {

					if (dist <= (float)range)
					{
						if (Keyboard::Get()->Press(VK_LSHIFT))
							vertices[index].Position.y -= power * Time::Delta();
						else
							vertices[index].Position.y += power * Time::Delta();
					}
				}
				if (type2 == 1)
				{
					float dx = vertices[index].Position.x - (LONG)position.x;
					float dz = vertices[index].Position.z - (LONG)position.z;
					float dist = sqrt(dx * dx + dz * dz);

					float perdist = dist / range;
					if (dist <= (float)range)
					{
						if (Keyboard::Get()->Press(VK_LSHIFT))
							vertices[index].Position.y -= cos(perdist) * power * Time::Delta();
						else
							vertices[index].Position.y += cos(perdist) * power * Time::Delta();
					}
				}
			}//for
		}//for
	}


	CreateNormalData();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::MiddleHeight(Vector3 &position, UINT type, UINT range, bool &fixed, float power)
{
	D3D11_RECT rect;
	rect.left = (LONG)position.x - range;
	rect.top = (LONG)position.z + range;
	rect.right = (LONG)position.x + range;
	rect.bottom = (LONG)position.z - range;

	// terrain의 끝쪽에서 닿을때 그 이상의 값에서 생성안되게
	if (rect.left < 0) rect.left = 0;
	if (rect.top >= height) rect.top = height;
	if (rect.right >= width) rect.right = width;
	if (rect.bottom < 0) rect.bottom = 0;


	for (LONG z = rect.bottom; z <= rect.top; z++)
	{
		for (LONG x = rect.left; x <= rect.right; x++)
		{
			UINT index = width * (UINT)z + (UINT)x;

			if (Keyboard::Get()->Press(VK_LSHIFT))
			{
				static float pixedY = 0.0f;

				if (fixed == false)
				{
					pixedY = GetHeight(position);
					fixed = true;
				}

				if (vertices[index].Position.y >= 0)
					vertices[index].Position.y = pixedY;
				else if (vertices[index].Position.y < 0)
					vertices[index].Position.y = pixedY;
			}
			else
			{
				if (vertices[index].Position.y >= 0)
					vertices[index].Position.y = power;
				else if (vertices[index].Position.y < 0)
					vertices[index].Position.y = power;
			}
		}
	}

	CreateNormalData();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::SlopeHeight(Vector3 & position, UINT type, UINT range, UINT range2, UINT direction, float power)
{
	D3D11_RECT rect;

	switch (direction)
	{
	case 0:	//L
		rect.left = (LONG)position.x - range;
		rect.top = (LONG)position.z + range2;
		rect.right = (LONG)position.x;
		rect.bottom = (LONG)position.z - range2;
		break;
	case 1:	//R
		rect.left = (LONG)position.x;
		rect.top = (LONG)position.z + range2;
		rect.right = (LONG)position.x + range;
		rect.bottom = (LONG)position.z - range2;
		break;
	case 2:	//U
		rect.left = (LONG)position.x - range2;
		rect.top = (LONG)position.z + range;
		rect.right = (LONG)position.x + range2;
		rect.bottom = (LONG)position.z;
		break;
	case 3:	//D
		rect.left = (LONG)position.x - range2;
		rect.top = (LONG)position.z;
		rect.right = (LONG)position.x + range2;
		rect.bottom = (LONG)position.z - range;
		break;

	}
	

	// terrain의 끝쪽에서 닿을때 그 이상의 값에서 생성안되게
	if (rect.left < 0) rect.left = 0;
	if (rect.top >= height) rect.top = height;
	if (rect.right >= width) rect.right = width;
	if (rect.bottom < 0) rect.bottom = 0;

	for (LONG z = rect.bottom; z <= rect.top; z++)
	{
		for (LONG x = rect.left; x <= rect.right; x++)
		{
			UINT index = width * (UINT)z + (UINT)x;

			if (direction == 0 || direction == 1)
			{
				float dx = vertices[index].Position.x - (LONG)position.x;
				float dy = vertices[index].Position.y - (LONG)position.y;
				float dist = sqrt(dx * dx + dy * dy);

				if (Mouse::Get()->Press(0))
				{
					if (Keyboard::Get()->Press(VK_LSHIFT))
						vertices[index].Position.y -= power * dist * Time::Delta();
					else
						vertices[index].Position.y += power * dist * Time::Delta();
				}
			}

			if (direction == 2 || direction == 3)
			{
				float dz = vertices[index].Position.z - (LONG)position.z;
				float dy = vertices[index].Position.y - (LONG)position.y;
				float dist = sqrt(dz * dz + dy * dy);

				if (Mouse::Get()->Press(0))
				{
					if (Keyboard::Get()->Press(VK_LSHIFT))
						vertices[index].Position.y -= power * dist * Time::Delta();
					else
						vertices[index].Position.y += power * dist * Time::Delta();
				}
			}
			

			
		}
	}

	CreateNormalData();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::CreateVertexData()
{
	vector<Color> heights;
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();


	vertexCount = width * height;
	vertices = new TerrainVertex[vertexCount];
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * z + x;
			UINT pixel = width * (height - 1 - z) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = (heights[pixel].r * 255.0f) / 10.0f;
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = (float)x / (float)width;
			vertices[index].Uv.y = (float)(height - 1 - z) / (float)height;
		}
	}
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * z + x;
			indices[index + 1] = width * (z + 1) + x;
			indices[index + 2] = width * z + x + 1;
			indices[index + 3] = width * z + x + 1;
			indices[index + 4] = width * (z + 1) + x;
			indices[index + 5] = width * (z + 1) + x + 1;

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		TerrainVertex v0 = vertices[index0];
		TerrainVertex v1 = vertices[index1];
		TerrainVertex v2 = vertices[index2];

		//NormalVector
		{
			Vector3 d1 = v1.Position - v0.Position;
			Vector3 d2 = v2.Position - v0.Position;

			Vector3 normal;
			D3DXVec3Cross(&normal, &d1, &d2);

			vertices[index0].Normal += normal;
			vertices[index1].Normal += normal;
			vertices[index2].Normal += normal;
		}
		
		//TangentVector
		{
			Vector3 d1 = v1.Position - v0.Position;
			Vector3 d2 = v2.Position - v0.Position;

			Vector2 u = v1.Uv - v0.Uv;
			Vector2 v = v2.Uv - v0.Uv;

			float d = 1.0f / (u.x * v.y - u.y * v.x);

			Vector3 tangent;
			tangent.x = (v.y * d1.x - v.x * d2.x) * d;
			tangent.y = (v.y * d1.y - v.x * d2.y) * d;
			tangent.z = (v.y * d1.z - v.x * d2.z) * d;

			vertices[index0].Tangent += tangent;
			vertices[index1].Tangent += tangent;
			vertices[index2].Tangent += tangent;
		}
	}

	for (UINT i = 0; i < vertexCount; i++)
	{
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
		D3DXVec3Normalize(&vertices[i].Tangent, &vertices[i].Tangent);
	}
		
}

void Terrain::CreateTriangleData()
{
	//삼각형의 갯수
	input = new InputDesc[indexCount / 3];
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].V0 = vertices[index0].Position;
		input[i].V1 = vertices[index1].Position;
		input[i].V2 = vertices[index2].Position;

		input[i].Index = i;
	}
}
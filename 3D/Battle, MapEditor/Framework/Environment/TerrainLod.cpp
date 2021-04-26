#include "Framework.h"
#include "TerrainLod.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

TerrainLod::TerrainLod(InitializeDesc & desc, bool modify)
	: Renderer(desc.shader), initDesc(desc), modify(modify)
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	sBaseMap = shader->AsSRV("BaseMap");

	map[0].sLayerMap = shader->AsSRV("LayerMap1");
	map[1].sLayerMap = shader->AsSRV("LayerMap2");
	map[2].sLayerMap = shader->AsSRV("LayerMap3");
	map[3].sLayerMap = shader->AsSRV("LayerSlope1");
	map[4].sLayerMap = shader->AsSRV("LayerSlope2");

	sHeightMap = shader->AsSRV("HeightMap");
	sNormalMap = shader->AsSRV("NormalMap");

	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = shader->AsConstantBuffer("CB_TerrainLod");

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_Brush");


	heightMap = new Texture(initDesc.heightMap);
	sHeightMap->SetResource(heightMap->SRV());
	heightColors = new float[heightMap->GetWidth() * heightMap->GetHeight()];
	heightColorsTemp = new float[heightMap->GetWidth() * heightMap->GetHeight()];
	heightMap->ReadPixel(DXGI_FORMAT_R32_FLOAT, heightColors);

	heightMap->CreateWritingTexture(DXGI_FORMAT_R32_FLOAT, 0);
	heightMap->CreateSRVTexture(DXGI_FORMAT_R32_FLOAT, 0);


	width = this->heightMap->GetWidth();
	height = this->heightMap->GetHeight();

	alphaMap = new Texture(initDesc.alphaMap);
	sAlphaMap = shader->AsSRV("AlphaMap");
	sAlphaMap->SetResource(alphaMap->SRV());
	//alphaMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &alphaMapPixel);

	alphaMap->CreateWritingTexture(DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	alphaMap->CreateSRVTexture(DXGI_FORMAT_R8G8B8A8_UNORM, 1);

	{
		ID3D11Texture2D* texture;
		texture = alphaMap->GetWritingTexture2();

		alphaMapPixel = new UINT[width * height];
		D3D11_MAPPED_SUBRESOURCE subResource;
		// texture좌표와 subREsurce좌표를 맞춘다.
		D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, NULL, &subResource);
		{
			// colors의 값에 subResource값을 넣는다.
			// 이것은 Terrain과 다르게 값을 가져오는 용도이다.
			memcpy(alphaMapPixel, subResource.pData, sizeof(UINT) * width * height);
		}
		D3D::GetDC()->Unmap(texture, 0);
	}
	


	vertexPerPatchX = (width / initDesc.CellsPerPatch) + 1;	//나머지 남는게 있을 것이기 때문에 +1해줌
	vertexPerPatchZ = (height / initDesc.CellsPerPatch) + 1;

	vertexCount = vertexPerPatchX * vertexPerPatchZ;
	faceCount = (vertexPerPatchX - 1) * (vertexPerPatchZ - 1);	//칸의 개수라 -1
	indexCount = faceCount * 4;


	CalcBoundY();
	CreateVertexData();
	CreateIndexData();

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexTerrain));	//
	indexBuffer = new IndexBuffer(indices, indexCount);

	bufferDesc.TexelCellSpaceU = 1.0f / (float)heightMap->GetWidth() - 1;	//한픽셀의 크기
	bufferDesc.TexelCellSpaceV = 1.0f / (float)heightMap->GetHeight() - 1;
	bufferDesc.HeightRatio = initDesc.HeightRatio;

	camera = new Fixity();
	perspective = new Perspective(D3D::Width(), D3D::Height(), 0.1f, 1000.0f, Math::PI * 0.35f);

	frustum = new Frustum(NULL, perspective);
	//frustum = new Frustum(camera, perspective);

	LayerMap(L"Terrain/Cliff (Grassy).jpg", 3);
	LayerMap(L"Terrain/Cliff (Layered Rock).jpg", 4);

	for (UINT i = 0; i < LayerCount; i++)
	{
		layerName[i] = L"";
	}
}

TerrainLod::~TerrainLod()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(brushBuffer);
	SafeDelete(buffer);
	SafeDelete(frustum);
	SafeDelete(camera);

	SafeDelete(heightMap);

	SafeDelete(baseMap);

	for (UINT i = 0; i < 5; i++)
		SafeDelete(map[i].layerMap);
}

void TerrainLod::Update()
{
	Super::Update();

	camera->Update();

	frustum->Update();
	frustum->Planes(bufferDesc.WorldFrustumPlanes);

	if (modify == false) return;
	//====================================================================
	ImGui::Begin("Editor");
	{
		ImGui::SliderFloat("MinDistance", &bufferDesc.MinDistance, 1, 100);
		ImGui::SliderFloat("MaxDistance", &bufferDesc.MaxDistance, 1, 1000);
		ImGui::Separator();

		//MousePos
		//if (Mouse::Get()->Press(0))
		{
			Vector3 pos = GetPickedPosition();
			pos.y = GetHeight(pos);
			pos.x = pos.x + width * 0.5f;
			pos.z = pos.z + height * 0.5f;
			ImGui::InputFloat3("currPos", pos);
		}

		//Splatting
		{
			static bool splattingOn = false;
			ImGui::Checkbox("Splatting", &splattingOn);
			if (splattingOn == true)
			{
				ImGui::Begin("SplattingEditor");
				{
					Splatting();

					static UINT splattingType = 0;
					ImGui::InputInt("BrushType", (int *)&brushDesc.Type);
					ImGui::InputInt("LayerType", (int *)&splattingType);
					ImGui::InputInt("LayerRange", (int *)&brushDesc.Range);
					ImGui::SliderFloat("ColorPower", &colorPower, 0.0f, 1.0f);
					brushDesc.Type %= 3;
					splattingType %= 3;
					brushDesc.Range %= 51;

					brushDesc.Location = GetPickedPosition();

					static bool clickOn = false;
					static bool eraseOn = false;
					ImGui::Checkbox("== ClickOn/Off : P ==", &clickOn);
					if (Keyboard::Get()->Down('P'))
					{
						if (clickOn == true)
							clickOn = false;
						else if (clickOn == false)
						{
							clickOn = true;
						}
					}

					ImGui::Checkbox("== EraseOn/Off : O ==", &eraseOn);
					if (Keyboard::Get()->Down('O'))
					{
						if (eraseOn == true)
						{
							eraseOn = false;
						}
						else if (eraseOn == false)
						{
							eraseOn = true;
							clickOn = false;
						}
					}

					if (brushDesc.Type == 1)
						ImGui::Text("== Square ==");
					else if (brushDesc.Type == 2)
						ImGui::Text("== Circle ==");

					if (clickOn == true)
					{
						eraseOn = false;
						SplattingApply(GetPickedPosition(), brushDesc.Type, splattingType, brushDesc.Range);
					}

					if (eraseOn == true)
					{
						SplattingErase(GetPickedPosition(), brushDesc.Type, brushDesc.Range);
					}
				}
				ImGui::End();
			}
		}

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

					if (heightPower <= 50.0f)
						ImGui::InputFloat("HeightPower", &heightPower, 0.1f);
					if (heightPower > 50.0f)
						heightPower = 50.0f;
					if (heightPower < 0.0f)
						heightPower = 0.0f;

					ImGui::InputInt("Range", (int *)&brushDesc.Range);
					brushDesc.Range %= 51;

					if (brushDesc.HeightType == 2)
						ImGui::InputInt("Range2", (int *)&brushDesc.Range2);
					brushDesc.Range2 %= 26;

					//Save Load
					{
						ImGui::InputInt("Save/LoadNum", (int *)&fileNum);
						fileNum %= 3;

						if (ImGui::Button("Save"))
						{
							switch (fileNum)
							{
							case 0:
								HeightSave(L"Height/heightTexture");
								break;
							case 1:
								HeightSave(L"Height/heightTexture1");
								break;
							case 2:
								HeightSave(L"Height/heightTexture2");
								break;
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("Load"))
						{
							switch (fileNum)
							{
							case 0:
								HeightLoad(L"Height/heightTexture");
								break;
							case 1:
								HeightLoad(L"Height/heightTexture1");
								break;
							case 2:
								HeightLoad(L"Height/heightTexture2");
								break;
							}
						}
					}

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
								RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range, heightPower);
							break;

						case 1:	//평탄화
							ImGui::Text("== Flatten ==");
							ImGui::Text("== 'Shift' : Curr/Power ==");

							if (Mouse::Get()->Press(0) && clickOn == true)
								MiddleHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range, fixed, heightPower);
							else
								fixed = false;
							break;

						case 2:		//경사로
							ImGui::Text("== Slope ==");
							ImGui::Text("== 'Shift' : Up/Down ==");
							ImGui::Text("== Range : X, Range2 : Y ==");
							ImGui::Text("== Input Direction ==");

							if (brushDesc.Type == 2) brushDesc.Type = 1;

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
							break;

						case 3:	//스무스
							ImGui::Text("== Smooth ==");
							if (Mouse::Get()->Press(0) && clickOn == true)
								SmoothHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
							break;
						}//switch
					}

				}
				ImGui::End();
			}
		}
	}
	ImGui::End();
}

void TerrainLod::Render()
{
	Super::Render();

	if (baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());

	for (UINT i = 0; i < 5; i++)
	{
		if (map[i].layerMap != NULL)
			map[i].sLayerMap->SetResource(map[i].layerMap->SRV());
	}

	if (normalMap != NULL)
		sNormalMap->SetResource(normalMap->SRV());


	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());
	brushBuffer->Apply();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());
	shader->DrawIndexed(0, Pass(), indexCount);
}

void TerrainLod::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void TerrainLod::LayerMap(wstring layer, UINT number)
{
	SafeDelete(map[number].layerMap);

	map[number].layerMap = new Texture(layer);
}

void TerrainLod::NormalMap(wstring file)
{
	SafeDelete(normalMap);

	normalMap = new Texture(file);
}

Vector3 TerrainLod::GetPickedPosition()
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

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * (z + 1) + x;
			index[2] = width * z + x + 1;
			index[3] = width * (z + 1) + x + 1;

			Vector3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;


			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		}
	}

	return Vector3(-1, FLT_MIN, -1);
}

float TerrainLod::GetHeight(Vector3 & position)
{
	UINT x = (int)position.x + (width * 0.5f);	//포지션은 -256~255  width는 512/2 = 256
	UINT z = (int)position.z + (height * 0.5f);

	//if (x < 0 || x > width - 1) return FLT_MIN;
	//if (z < 0 || z > height - 1) return FLT_MIN;
	if (InBounds(x, z) == false)
		return FLT_MIN;

	UINT pixel = width * (height - z) + x;

	return heightColors[pixel] * 255.0f / bufferDesc.HeightRatio;
}

bool TerrainLod::InBounds(UINT x, UINT z)
{
	return x >= 0 && x < width - 1 && z >= 0 && z < height - 1;
}

void TerrainLod::CalcPatchBounds(UINT x, UINT z)
{
	UINT x0 = x * initDesc.CellsPerPatch;
	UINT x1 = (x + 1) * initDesc.CellsPerPatch;

	UINT z0 = z * initDesc.CellsPerPatch;
	UINT z1 = (z + 1) * initDesc.CellsPerPatch;


	float minY = FLT_MAX;
	float maxY = FLT_MIN;

	for (UINT z = z0; z <= z1; z++)
	{
		for (UINT x = x0; x <= x1; x++)
		{
			float y = 0.0f;
			UINT pixel = width * (height - 1 - z) + x;

			if (InBounds(x, z))
				y = heightColors[pixel] * 255 / initDesc.HeightRatio;

			minY = min(minY, y);
			maxY = max(maxY, y);
		}
	}

	UINT patchID = (vertexPerPatchX - 1) * z + x;
	bounds[patchID] = Vector2(minY, maxY);
}

void TerrainLod::CalcBoundY()
{
	bounds.assign(faceCount, Vector2());

	for (UINT z = 0; z < vertexPerPatchZ - 1; z++)
	{
		for (UINT x = 0; x < vertexPerPatchX - 1; x++)
		{
			CalcPatchBounds(x, z);
		}
	}
}

void TerrainLod::CreateVertexData()
{
	vertices = new VertexTerrain[vertexCount];

	float halfWidth = (float)width * 0.5f;
	float halfHeight = (float)height * 0.5f;

	float patchWidth = (float)width / (float)(vertexPerPatchX - 1);
	float patchHeight = (float)height / (float)(vertexPerPatchZ - 1);

	float u = 1.0f / (float)(vertexPerPatchX - 1);
	float v = 1.0f / (float)(vertexPerPatchZ - 1);
	//float id = 0;
	for (UINT z = 0; z < vertexPerPatchZ; z++)
	{
		float z1 = halfHeight - (float)z * patchHeight;

		for (UINT x = 0; x < vertexPerPatchX; x++)
		{
			float x1 = -halfWidth + (float)x * patchWidth;
			UINT patchId = vertexPerPatchX * z + x;

			vertices[patchId].Position = Vector3(x1, 0, z1);
			vertices[patchId].Uv = Vector2(x * u, z * v);
			//vertices[patchId].ID = Vector3(id, 0, 0);
			//id += 0.001f;
		}
	}

	for (UINT z = 0; z < vertexPerPatchZ - 1; z++)
	{
		for (UINT x = 0; x < vertexPerPatchX - 1; x++)
		{
			UINT patchID = (vertexPerPatchX - 1) * z + x;
			UINT vertID = vertexPerPatchX * z + x;

			vertices[vertID].BoundsY = bounds[patchID];
		}
	}
}

void TerrainLod::CreateIndexData()
{
	UINT index = 0;
	this->indices = new UINT[indexCount];
	for (UINT z = 0; z < vertexPerPatchZ - 1; z++)
	{
		for (UINT x = 0; x < vertexPerPatchX - 1; x++)
		{
			this->indices[index++] = vertexPerPatchX * z + x;
			this->indices[index++] = vertexPerPatchX * z + x + 1;
			this->indices[index++] = vertexPerPatchX * (z + 1) + x;
			this->indices[index++] = vertexPerPatchX * (z + 1) + x + 1;
		}
	}
}

void TerrainLod::RaiseHeight(Vector3 &position, UINT type, UINT range, float power)
{
	UINT x = (int)position.x + (width * 0.5f);
	UINT z = (int)position.z + (height * 0.5f);

	D3D11_RECT rect;
	rect.left = x - range;
	rect.top = z + range;
	rect.right = x + range;
	rect.bottom = z - range;

	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width - 1) rect.right = (LONG)width - 1;
	if (rect.bottom < 0) rect.bottom = 0;
	
	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;

			switch (type)
			{
			case 1:
				if (Keyboard::Get()->Press(VK_LSHIFT))
					heightColors[index] -= power / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
				else
					heightColors[index] += power / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
				break;
			case 2:
				float dxi = x;
				float dzi = z;

				float dx = (float)xi - dxi;
				float dz = (float)zi - dzi;
				float dist = sqrt(dx * dx + dz * dz);

				if (dist <= (float)range)
				{
					float ratio = dist / range;
					float theta = (Math::PI / 2.0f) * ratio;
					float forceValue = cos(theta);

					if (Keyboard::Get()->Press(VK_LSHIFT))
						heightColors[index] -= (forceValue / 2.0f) * power / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
					else
						heightColors[index] += (forceValue / 2.0f) * power / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
				}
				break;
			}

		}//for
	}//for

	TextureApply();
}

void TerrainLod::MiddleHeight(Vector3 &position, UINT type, UINT range, bool &fixed, float power)
{
	UINT x = (int)position.x + (width * 0.5f);
	UINT z = (int)position.z + (height * 0.5f);

	D3D11_RECT rect;
	rect.left = x - range;
	rect.top = z + range;
	rect.right = x + range;
	rect.bottom = z - range;


	// terrain의 끝쪽에서 닿을때 그 이상의 값에서 생성안되게
	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width) rect.right = (LONG)width;
	if (rect.bottom < 0) rect.bottom = 0;


	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;

			switch (type)
			{
			case 1:
				if (Keyboard::Get()->Press(VK_LSHIFT))
				{
					static float pixedY = 0.0f;

					if (fixed == false)
					{
						pixedY = GetHeight(position);
						fixed = true;
					}

					heightColors[index] = pixedY / 255.0f * initDesc.HeightRatio;
				}
				else
				{
					heightColors[index] = power / 255.0f * initDesc.HeightRatio;
				}
				break;
			case 2:
				float dxi = x;
				float dzi = z;

				float dx = (float)xi - dxi;
				float dz = (float)zi - dzi;
				float dist = sqrt(dx * dx + dz * dz);

				if (dist <= (float)range)
				{
					if (Keyboard::Get()->Press(VK_LSHIFT))
					{
						static float pixedY = 0.0f;

						if (fixed == false)
						{
							pixedY = GetHeight(position);
							fixed = true;
						}

						heightColors[index] = pixedY / 255.0f * initDesc.HeightRatio;
					}
					else
					{
						heightColors[index] = power / 255.0f * initDesc.HeightRatio;
					}
				}
				break;
			}
		}
	}

	TextureApply();
}

void TerrainLod::SlopeHeight(Vector3 & position, UINT type, UINT range, UINT range2, UINT direction, float power)
{
	UINT x = (int)position.x + (width * 0.5f);
	UINT z = (int)position.z + (height * 0.5f);

	D3D11_RECT rect;

	switch (direction)
	{
	case 0:	//L
		rect.left = x - range;
		rect.top = z + range2;
		rect.right = x;
		rect.bottom = z - range2;
		break;
	case 1:	//R
		rect.left = x;
		rect.top = z + range2;
		rect.right = x + range;
		rect.bottom = z - range2;
		break;
	case 2:	//U
		rect.left = x - range2;
		rect.top = z + range;
		rect.right = x + range2;
		rect.bottom = z;
		break;
	case 3:	//D
		rect.left = x - range2;
		rect.top = z;
		rect.right = x + range2;
		rect.bottom = z - range;
		break;
	}

	// terrain의 끝쪽에서 닿을때 그 이상의 값에서 생성안되게
	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width) rect.right = (LONG)width;
	if (rect.bottom < 0) rect.bottom = 0;

	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;

			if (direction == 0 || direction == 1)
			{
				float dxi = x;
				float dx = (float)xi - dxi;
				float dy = (heightColors[index] * 255.0f / initDesc.HeightRatio) - GetHeight(position);
				float dist = sqrt(dx * dx + dy * dy);

				if (Mouse::Get()->Press(0))
				{
					if (Keyboard::Get()->Press(VK_LSHIFT))
						heightColors[index] -= power * dist / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
					else
						heightColors[index] += power * dist / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
				}
			}

			if (direction == 2 || direction == 3)
			{
				float dzi = z;
				float dz = (float)zi - dzi;
				float dy = (heightColors[index] * 255.0f / initDesc.HeightRatio) - GetHeight(position);
				float dist = sqrt(dz * dz + dy * dy);

				if (Mouse::Get()->Press(0))
				{
					if (Keyboard::Get()->Press(VK_LSHIFT))
						heightColors[index] -= power * dist / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
					else
						heightColors[index] += power * dist / 255.0f * initDesc.HeightRatio * Time::Get()->Delta();
				}
			}
		}
	}

	TextureApply();
}

void TerrainLod::SmoothHeight(Vector3 & position, UINT type, UINT range)
{
	static float timer = 0;
	timer += Time::Get()->Delta();

	if (timer < 0.1f)
		return;

	timer = 0.0f;

	UINT x = (int)position.x + (width * 0.5f);
	UINT z = (int)position.z + (height * 0.5f);

	D3D11_RECT rect;
	rect.left = x - range;
	rect.top = z + range;
	rect.right = x + range;
	rect.bottom = z - range;

	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width) rect.right = (LONG)width;
	if (rect.bottom < 0) rect.bottom = 0;


	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;

			int adjacentSections = 0;
			float sectionsTotal = 0.0f;

			UINT indexMinY = width * (height - (UINT)zi - 1) + (UINT)xi;
			UINT indexMaxY = width * (height - (UINT)zi + 1) + (UINT)xi;

			if ((index - 1) > 0) // Check to left
			{
				sectionsTotal += heightColors[index - 1];
				adjacentSections++;

				if (indexMinY > 0) // Check up and to the left
				{
					sectionsTotal += heightColors[indexMinY - 1];
					adjacentSections++;
				}

				if (indexMaxY < height) // Check down and to the left
				{
					sectionsTotal += heightColors[indexMaxY - 1];
					adjacentSections++;
				}
			}

			if ((index + 1) < width) // Check to right
			{
				sectionsTotal += heightColors[index + 1];
				adjacentSections++;

				if (indexMinY > 0) // Check up and to the right
				{
					sectionsTotal += heightColors[indexMinY + 1];
					adjacentSections++;
				}

				if (indexMaxY < height) // Check down and to the right
				{
					sectionsTotal += heightColors[indexMaxY + 1];
					adjacentSections++;
				}
			}

			if (indexMinY > 0) // Check above
			{
				sectionsTotal += heightColors[indexMinY];
				adjacentSections++;
			}

			if (indexMaxY < height) // Check below
			{
				sectionsTotal += heightColors[indexMaxY];
				adjacentSections++;
			}

			heightColorsTemp[index] = (heightColors[index] + (sectionsTotal / adjacentSections)) * 0.5f;
		}
	}

	// Overwrite the HeightData info with our new smoothed info
	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;
			heightColors[index] = heightColorsTemp[index];
		}
	}

	TextureApply();
}

void TerrainLod::HeightSave(wstring savePath)
{
	//HeightSave
	{
		savePath = L"../../_Textures/" + savePath + L".png";
		Path::CreateFolders(Path::GetDirectoryName(savePath));

		BinaryWriter* w = new BinaryWriter();
		w->Open(savePath);
		
		for (UINT i = 0; i < width * height; i++)
		{
			w->Float(heightColors[i]);
		}

		w->Close();
		SafeDelete(w);
	}
}

void TerrainLod::HeightLoad(wstring savePath)
{
	//AlphaMap Load & Splatting Texture Load
	{
		savePath = L"../../_Textures/" + savePath + L".png";

		BinaryReader* r = new BinaryReader();
		r->Open(savePath);

		for (UINT i = 0; i < width * height; i++)
		{
			heightColors[i] = r->Float();
		}

		r->Close();
		SafeDelete(r);
	}

	TextureApply();
}

void TerrainLod::TextureApply()
{
	ID3D11Texture2D* texture = heightMap->GetWritingTexture();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE, NULL, &subResource);
	{
		memcpy(subResource.pData, heightColors, sizeof(float) * width * height);
	}
	D3D::GetDC()->Unmap(texture, 0);

	heightMap->ChangeSRVTexture(DXGI_FORMAT_R32_FLOAT, texture, 0);
	sHeightMap->SetResource(heightMap->SRV());
}

void TerrainLod::TextureApply2()
{
	ID3D11Texture2D* texture = alphaMap->GetWritingTexture2();

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE, NULL, &subResource);
	{
		memcpy(subResource.pData, alphaMapPixel, sizeof(UINT) * width * height);
	}
	D3D::GetDC()->Unmap(texture, 0);

	alphaMap->ChangeSRVTexture(DXGI_FORMAT_R8G8B8A8_UNORM, texture, 1);
	sAlphaMap->SetResource(alphaMap->SRV());
}

void TerrainLod::Splatting()
{
	std::wstring file;
	if (ImGui::Button("BaseLayer"))
	{
		Path::OpenFileDialog(file, Path::ImageFilter, L"../../_Textures/Terrain", [=](std::wstring path)
		{
			string folder = String::ToString(L"../../_Textures/Terrain");
			layerName[0] = path;
			if (layerName[0] != L"")
			{
				setLayer[0] = true;
			}
			BaseMap(layerName[0]);
		});
	}
	if (ImGui::Button("Layer1"))
	{
		Path::OpenFileDialog(file, Path::ImageFilter, L"../../_Textures/Terrain", [=](std::wstring path)
		{
			string folder = String::ToString(L"../../_Textures/Terrain");
			layerName[1] = path;
			if (layerName[1] != L"")
			{
				setLayer[1] = true;
			}
			LayerMap(layerName[1], 0);
		});
	}
	if (ImGui::Button("Layer2"))
	{
		Path::OpenFileDialog(file, Path::ImageFilter, L"../../_Textures/Terrain", [=](std::wstring path)
		{
			string folder = String::ToString(L"../../_Textures/Terrain");
			layerName[2] = path;
			if (layerName[2] != L"")
			{
				setLayer[2] = true;
			}
			LayerMap(layerName[2], 1);
		});
	}
	if (ImGui::Button("Layer3"))
	{
		Path::OpenFileDialog(file, Path::ImageFilter, L"../../_Textures/Terrain", [=](std::wstring path)
		{
			string folder = String::ToString(L"../../_Textures/Terrain");
			layerName[3] = path;
			if (layerName[3] != L"")
			{
				setLayer[3] = true;
			}
			LayerMap(layerName[3], 2);
		});
	}

	//Splatting Save & Load
	{
		ImGui::InputInt("Save/LoadNum", (int *)&fileNum);
		fileNum %= 3;

		if (ImGui::Button("Save"))
		{
			switch (fileNum)
			{
			case 0:
				SplattingSave(L"Splatting/splattingTexture");
				break;
			case 1:
				SplattingSave(L"Splatting/splattingTexture1");
				break;
			case 2:
				SplattingSave(L"Splatting/splattingTexture2");
				break;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			switch (fileNum)
			{
			case 0:
				SplattingLoad(L"Splatting/splattingTexture");
				break;
			case 1:
				SplattingLoad(L"Splatting/splattingTexture1");
				break;
			case 2:
				SplattingLoad(L"Splatting/splattingTexture2");
				break;
			}
		}
	}
	
}

void TerrainLod::SplattingApply(Vector3 &position, UINT type, UINT type2, UINT range)
{
	UINT x = (int)position.x + (width * 0.5f);
	UINT z = (int)position.z + (height * 0.5f);

	D3D11_RECT rect;
	rect.left = x - range;
	rect.top = z + range;
	rect.right = x + range;
	rect.bottom = z - range;

	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width) rect.right = (LONG)width;
	if (rect.bottom < 0) rect.bottom = 0;

	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;

			UINT TileX_G = ((0x00FF0000 & alphaMapPixel[index]) >> 16);	//b
			UINT TileX_B = ((0x0000FF00 & alphaMapPixel[index]) >> 8);	//g
			UINT TileX_A = ((0x000000FF & alphaMapPixel[index]) >> 0);	//r

			switch (type)
			{
			case 1:
				switch (type2)
				{
				case 0:
					if (Mouse::Get()->Press(0) && setLayer[1] == true)
					{
						TileX_A = 255 * colorPower;

						if (TileX_B == 255)
							TileX_B = 0 << 8;
						else
							TileX_B = TileX_B << 8;

						if (TileX_G == 255)
							TileX_G = 0 << 16;
						else
							TileX_G = TileX_G << 16;

						alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
					}
					break;
				case 1:
					if (Mouse::Get()->Press(0) && setLayer[2] == true)
					{

						if (TileX_A == 255)
							TileX_A = 0 << 0;
						else
							TileX_A = TileX_A << 0;

						TileX_B = 255 * colorPower;
						TileX_B = TileX_B << 8;

						if (TileX_G == 255)
							TileX_G = 0 << 16;
						else
							TileX_G = TileX_G << 16;

						alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
					}
					break;
				case 2:
					if (Mouse::Get()->Press(0) && setLayer[3] == true)
					{
						if (TileX_A == 255)
							TileX_A = 0 << 0;
						else
							TileX_A = TileX_A << 0;

						if (TileX_B == 255)
							TileX_B = 0 << 8;
						else
							TileX_B = TileX_B << 8;

						TileX_G = 255 * colorPower;
						TileX_G = TileX_G << 16;

						alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
					}
					break;
				}
				break;
			case 2:
				float dxi = x;
				float dzi = z;

				float dx = (float)xi - dxi;
				float dz = (float)zi - dzi;
				float dist = sqrt(dx * dx + dz * dz);

				if (dist <= (float)range)
				{
					switch (type2)
					{
					case 0:
						if (Mouse::Get()->Press(0) && setLayer[1] == true)
						{
							TileX_A = 255 * colorPower;

							if (TileX_B == 255)
								TileX_B = 0 << 8;
							else
								TileX_B = TileX_B << 8;

							if (TileX_G == 255)
								TileX_G = 0 << 16;
							else
								TileX_G = TileX_G << 16;

							alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
						}
						break;
					case 1:
						if (Mouse::Get()->Press(0) && setLayer[2] == true)
						{
							if (TileX_A == 255)
								TileX_A = 0 << 0;
							else
								TileX_A = TileX_A << 0;

							TileX_B = 255 * colorPower;
							TileX_B = TileX_B << 8;

							if (TileX_G == 255)
								TileX_G = 0 << 16;
							else
								TileX_G = TileX_G << 16;

							alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
						}
						break;
					case 2:
						if (Mouse::Get()->Press(0) && setLayer[3] == true)
						{
							if (TileX_A == 255)
								TileX_A = 0 << 0;
							else
								TileX_A = TileX_A << 0;

							if (TileX_B == 255)
								TileX_B = 0 << 8;
							else
								TileX_B = TileX_B << 8;

							TileX_G = 255 * colorPower;
							TileX_G = TileX_G << 16;

							alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
						}
						break;
					}
				}
				break;
			}

		}//for
	}//for

	TextureApply2();
}

void TerrainLod::SplattingErase(Vector3 & position, UINT type, UINT range)
{
	UINT x = (int)position.x + (width * 0.5f);
	UINT z = (int)position.z + (height * 0.5f);

	D3D11_RECT rect;
	rect.left = x - range;
	rect.top = z + range;
	rect.right = x + range;
	rect.bottom = z - range;

	if (rect.left < 0) rect.left = 0;
	if (rect.top >= (LONG)height) rect.top = (LONG)height;
	if (rect.right >= (LONG)width) rect.right = (LONG)width;
	if (rect.bottom < 0) rect.bottom = 0;

	for (LONG zi = rect.bottom; zi <= rect.top; zi++)
	{
		for (LONG xi = rect.left; xi <= rect.right; xi++)
		{
			UINT index = width * (height - (UINT)zi) + (UINT)xi;

			UINT TileX_G = ((0x00FF0000 & alphaMapPixel[index]) >> 16);	//b
			UINT TileX_B = ((0x0000FF00 & alphaMapPixel[index]) >> 8);	//g
			UINT TileX_A = ((0x000000FF & alphaMapPixel[index]) >> 0);	//r

			switch (type)
			{
			case 1:
				if (Mouse::Get()->Press(0))
				{
					TileX_A = 0 << 0;
					TileX_B = 0 << 8;
					TileX_G = 0 << 16;

					alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
				}
				break;
			case 2:
				float dxi = x;
				float dzi = z;

				float dx = (float)xi - dxi;
				float dz = (float)zi - dzi;
				float dist = sqrt(dx * dx + dz * dz);

				if (dist <= (float)range)
				{
					if (Mouse::Get()->Press(0))
					{
						TileX_A = 0 << 0;
						TileX_B = 0 << 8;
						TileX_G = 0 << 16;

						alphaMapPixel[index] = TileX_G + TileX_B + TileX_A;
					}
				}
				break;
			}

		}//for
	}//for

	TextureApply2();
}

void TerrainLod::SplattingSave(wstring savePath)
{
	//AlphaMap Save & Splatting Texture Save
	{
		savePath = L"../../_Textures/" + savePath + L".png";
		Path::CreateFolders(Path::GetDirectoryName(savePath));

		string file[4];
		for (UINT i = 0; i < LayerCount; i++)
		{
			file[i] = String::ToString(Path::GetFileName(layerName[i]));
		}

		BinaryWriter* w = new BinaryWriter();
		w->Open(savePath);

		for (UINT i = 0; i < width * height; i++)
		{
			w->UInt(alphaMapPixel[i]);
		}

		for (int i = 0; i < LayerCount; i++)
			w->String(file[i]);

		w->Close();
		SafeDelete(w);
	}

}

void TerrainLod::SplattingLoad(wstring savePath)
{
	//AlphaMap Load & Splatting Texture Load
	{
		savePath = L"../../_Textures/" + savePath + L".png";

		BinaryReader* r = new BinaryReader();
		r->Open(savePath);

		for (UINT i = 0; i < width * height; i++)
		{
			alphaMapPixel[i] = r->UInt();
		}


		for (UINT i = 0; i < LayerCount; i++)
		{
			layerName[i] = String::ToWString(r->String());

			if (i == 0)
			{
				if (layerName[0] != L"")
					BaseMap(L"Terrain/" + layerName[0]);
			}
			else
			{
				if (layerName[i] != L"")
					LayerMap(L"Terrain/" + layerName[i], i);
			}
		}

		r->Close();
		SafeDelete(r);
	}

	TextureApply2();
}



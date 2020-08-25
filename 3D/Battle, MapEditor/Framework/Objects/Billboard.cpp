#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(Shader * shader)
	: Renderer(shader)
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	sMap = shader->AsSRV("Maps");
}

Billboard::~Billboard()
{
	SafeDelete(textures);
}

void Billboard::Add(Vector3 & position, Vector2 & scale, UINT & count)
{
	VertexScale vertex;
	vertex.Position = position;
	vertex.Scale = scale;
	vertex.Count = count;

	vertices.push_back(vertex);
}

void Billboard::AddTexture(wstring file)
{
	SafeDelete(textures);

	textureFiles.push_back(file);
}

void Billboard::Update()
{
	Super::Update();
}

void Billboard::Render()
{
	if (vertices.size() != vertexCount)
	{
		vertexCount = vertices.size();

		if (vertexCount > 0)	//오류해결했음
		{
			SafeDelete(vertexBuffer);
			vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(VertexScale));
		}
	}

	if (textureFiles.size() > 0 && textures == NULL)
		textures = new TextureArray(textureFiles);

	Super::Render();

	sMap->SetResource(textures->SRV());
	shader->Draw(0, Pass(), vertexCount);	//위치만 넣음
}
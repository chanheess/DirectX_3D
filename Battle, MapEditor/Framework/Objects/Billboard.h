#pragma once

class Billboard : public Renderer
{
public:
	Billboard(Shader* shader);
	~Billboard();

	void Add(Vector3& position, Vector2& scale, UINT& count);
	void AddTexture(wstring file);


	void Update();
	void Render();

private:
	struct VertexScale
	{
		Vector3 Position;
		Vector2 Scale;	//면의 크기
		UINT Count;
	};

private:
	vector<VertexScale> vertices;

	vector<wstring> textureFiles;
	TextureArray* textures = NULL;
	ID3DX11EffectShaderResourceVariable* sMap;

public:
	vector<wstring> SaveTexture() { return textureFiles; }
	vector<VertexScale> SaveScale() { return vertices; }
	void Erase(int i) { vertices.erase(vertices.begin() + i); }
};
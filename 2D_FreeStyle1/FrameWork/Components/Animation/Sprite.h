#pragma once
#include "./Components/Component.h"

class Sprite : public Component
{
public:
	Sprite();
	Sprite(wstring textureFile);
	Sprite(wstring textureFile, UINT startX, UINT startY);
	Sprite(wstring textureFile, UINT startX, UINT startY,
		UINT endX, UINT endY);

	~Sprite();

	void Update();
	void Render();

	void Insfactor() override;

	void SaveSprite(Xml::XMLElement* node);

	void Diffuse(D3DXCOLOR color) { shader->DiffuseColor(color); }
public:
	//----------------------------------------------------------------
	//	Vertex
	//----------------------------------------------------------------
	void Uv(D3DXVECTOR2 vec);
	//-----------------------------------------------------------------
	//	Material (재질 정보)
	//-----------------------------------------------------------------

	float GetWidth() { return width; }
	float GetHeight() { return height; }
private:

	void Initialize(
		wstring textureFile, float startX, float startY,
		float endX, float endY);

private:
	class VBuffer * vertexBuffer;
	class IBuffer * indexBuffer;

	class Texture * texture;
	class Shader* shader;

private:
	VertexTexture * vertices;
	wstring file;

	Vector2 offset;
	Vector2 count;

	float width, height;
};


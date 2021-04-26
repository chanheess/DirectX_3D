#pragma once
//fileName : Vertex.h

//VertexShader -> º¸³¾ vertex

struct Vertex
{
	D3DXVECTOR3 Position;
	Vertex()
	{
		Position = D3DXVECTOR3(0, 0, 0);
	}
	Vertex(float x, float y)
	{
		Position = D3DXVECTOR3(x, y, 0);
	}
	Vertex(D3DXVECTOR3 vec)
	{
		Position = vec;
	}

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static const UINT count;
};

struct VertexTexture
{
	VertexTexture() : Position(0, 0, 0), Uv(0, 0) {}
	VertexTexture(float x, float y, float u, float v)
		:Position(x, y, 0.0f), Uv(u, v) {}
	VertexTexture(D3DXVECTOR3 position, D3DXVECTOR2 uv)
		:Position(position), Uv(uv) {}

	D3DXVECTOR3 Position;
	D3DXVECTOR2 Uv;

	static D3D11_INPUT_ELEMENT_DESC desc[];
	static const UINT count;
};

typedef struct VertexTextureInstancePosition
{
	static D3D11_INPUT_ELEMENT_DESC desc[];
	static const UINT count;

}VTIP;

typedef struct VertexTextureInstancePositionColor
{
	static D3D11_INPUT_ELEMENT_DESC desc[];
	static const UINT count;

}VTIPC;

typedef struct VertexTextureInstancePositionTileUv
{
	static D3D11_INPUT_ELEMENT_DESC desc[];
	static const UINT count;

}VTIPT;
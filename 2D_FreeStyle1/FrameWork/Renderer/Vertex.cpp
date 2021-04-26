#include "stdafx.h"
#include "Vertex.h"

#define FLOAT2 DXGI_FORMAT_R32G32_FLOAT
#define FLOAT3 DXGI_FORMAT_R32G32B32_FLOAT
#define FLOAT4 DXGI_FORMAT_R32G32B32A32_FLOAT

#define VERTEX D3D11_INPUT_PER_VERTEX_DATA
#define INSTANCE D3D11_INPUT_PER_INSTANCE_DATA

#define ALIGEND D3D11_APPEND_ALIGNED_ELEMENT
//---------------------------------------------------------------------
//	Vertex
//---------------------------------------------------------------------
D3D11_INPUT_ELEMENT_DESC Vertex::desc[] =
{
	{ "POSITION", 0, FLOAT3, 0, 0, VERTEX, 0},
};
UINT const Vertex::count = 1;

//---------------------------------------------------------------------
//	Vertex	Texture
//---------------------------------------------------------------------
D3D11_INPUT_ELEMENT_DESC VertexTexture::desc[] =
{
	{ "POSITION", 0, FLOAT3, 0, 0, VERTEX, 0 },
	{ "UV", 0 , FLOAT2, 0, ALIGEND, VERTEX, 0},
};
UINT const VertexTexture::count = 2;

//---------------------------------------------------------------------
//	Vertex	Texture	 Intance Position
//---------------------------------------------------------------------
D3D11_INPUT_ELEMENT_DESC VertexTextureInstancePosition::desc[] =
{
	{ "POSITION", 0, FLOAT3, 0, 0, VERTEX, 0 },
	{ "UV", 0 , FLOAT2, 0, ALIGEND, VERTEX, 0 },
	{ "INDEX", 0 , FLOAT3, 1, 0,
		D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};
UINT const VertexTextureInstancePosition::count = 3;
//---------------------------------------------------------------------
//	Vertex	Texture	 Intance Position Color
//---------------------------------------------------------------------
D3D11_INPUT_ELEMENT_DESC VertexTextureInstancePositionColor::desc[] =
{
	{ "POSITION", 0, FLOAT3, 0, 0, VERTEX, 0 },
	{ "UV", 0 , FLOAT2, 0, ALIGEND, VERTEX, 0 },
	{ "COORD", 0 , FLOAT2, 1, 0, INSTANCE, 1 },
	{ "COLOR", 0 , FLOAT4, 1, ALIGEND, INSTANCE, 1 },
};
UINT const VertexTextureInstancePositionColor::count = 4;

//---------------------------------------------------------------------
//	Vertex	Texture	 Intance Position TileUv
//---------------------------------------------------------------------
D3D11_INPUT_ELEMENT_DESC VertexTextureInstancePositionTileUv::desc[] =
{
	{ "POSITION", 0, FLOAT3, 0, 0, VERTEX, 0 },
	{ "UV", 0 , FLOAT2, 0, ALIGEND, VERTEX, 0 },
	{ "INDEX", 0 , FLOAT3, 1, 0, INSTANCE, 1 },
	{ "TILEUV", 0 , FLOAT4, 1, ALIGEND, INSTANCE, 1 },
};
UINT const VertexTextureInstancePositionTileUv::count = 4;
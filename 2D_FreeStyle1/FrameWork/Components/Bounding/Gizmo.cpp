#include "stdafx.h"
#include "Gizmo.h"
#include "./Renderer/Buffers/CBuffer.h"
#include "./Renderer/Buffers/VBuffer.h"

Gizmo::Gizmo()
	: vertexBuffer(NULL)
{
	shader = new Shader(L"Color.hlsl");

	struct CB_Color
	{
		D3DXCOLOR color = 0xFF00FF00;
	}data;
	colorBuffer = new CBuffer(
		&data, sizeof(CB_Color), BufferType::PS
	);

	struct CB_World
	{
		D3DXMATRIX data;
	};
	CB_World worldDesc; //도형의 위치 크기 회전
	D3DXMatrixIdentity(&worldDesc.data);
	worldBuffer = new CBuffer(&worldDesc, sizeof(CB_World));

	vertexBuffer = new VBuffer
	(
		NULL, sizeof(VertexTexture), 60
	);
}



Gizmo::~Gizmo()
{
}

Gizmo* Gizmo::instance = NULL;

Gizmo & Gizmo::Get()
{
	if (instance == NULL)
		instance = new Gizmo;
	return *instance;
}

void Gizmo::Delete()
{
	SAFE_DELETE(instance);
}

void Gizmo::Update()
{
	if (vertexBuffer != NULL)
		vertexBuffer->Change();
}

void Gizmo::Render()
{
	if (vertexBuffer != NULL)
	{
		shader->Render();
		worldBuffer->Render();
		colorBuffer->Render();
		vertexBuffer->Render();
		D2D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		D2D::GetDC()->Draw(vertices.size(), 0);
	}
}

void Gizmo::DrawLine(ILine * line, D3DXMATRIX world)
{
	line->GetLine(&vertices, world);
	vertexBuffer->SetData(&vertices[0]);
	Render();
}


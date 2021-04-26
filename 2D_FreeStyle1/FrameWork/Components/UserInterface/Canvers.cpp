#include "stdafx.h"
#include "Canvers.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
#include "./Renderer/Buffers/CBuffer.h"
Canvers::Canvers()
{
	tag = Tag::UserInterface;
	Struct data;

	D3DXMatrixIdentity(&data.View);

	D2DDesc desc;
	D2D::GetDesc(&desc);

	D3DXMatrixOrthoOffCenterLH(
		&data.Ortho, 0, desc.Width,
		 0, desc.Height, 0.0f, 1.0f
	);
	D3DXMatrixTranspose(&data.Ortho, &data.Ortho);

	buffer = new CBuffer(&data, sizeof(Struct));
}


Canvers::~Canvers()
{
}

void Canvers::Ready()
{
	__super::Ready();
}

void Canvers::Update()
{
	__super::Update();
}

void Canvers::Render()
{
	buffer->Render();

	__super::Render();
}

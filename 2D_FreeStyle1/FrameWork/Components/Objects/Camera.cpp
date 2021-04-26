#include "stdafx.h"
#include "Camera.h"
#include "./GameObjects/GameObject.h"
#include "./Components/Objects/Transform.h"
#include "./Renderer/Buffers/CBuffer.h"
Camera::Camera(float* width, float* height)
	: Component(L"Camera"), bUI(false)
{
	D2DDesc d2ddesc;
	D2D::GetDesc(&d2ddesc);

	if (width == NULL) this->width = d2ddesc.Width;
	else this->width = *width;
	if (height == NULL) this->height = d2ddesc.Height;
	else this->height = *height;

	D3DXMatrixLookAtLH
	(
		&viewDesc.View,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 1),
		&D3DXVECTOR3(0, 1, 0)
	);
	
	D3DXMatrixOrthoOffCenterLH
	(
		&viewDesc.Ortho,
		0, this->width,
		0, this->height,
		0.0f, 1.0f
	);

	D3DXMatrixTranspose(&viewDesc.View, &viewDesc.View);
	D3DXMatrixTranspose(&viewDesc.Ortho, &viewDesc.Ortho);

	viewBuffer = new CBuffer
	(
		&viewDesc,
		sizeof(CB_ViewOrtho),
		BufferType::VS
	);
	viewBuffer->Render();
	shake = Vector3(0, 0, 0);
	bShake = false;

	transform = NULL;
}


Camera::~Camera()
{
}

void Camera::Ready()
{
	transform = gameObject->transform;
	prev = transform->Position(Space::World);

	if (bUI)
	{
		D3DXMatrixIdentity(&viewDesc.View);
	}
}

void Camera::Update()
{
	Vector3 position = transform->Position(Space::World);

	if (!VEC3EQUAL(prev, position) || bChange)
	{
		if (!bUI)
		{
			Vector3 scale = transform->Scale(Space::World);
			D3DXMatrixLookAtLH
			(
				&viewDesc.View,
				&(position - scale),
				&(position - scale + Vector3(0, 0, 1)),
				&D3DXVECTOR3(0, 1, 0)
			);
			D3DXMatrixTranspose(&viewDesc.View, &viewDesc.View);
		}
		viewBuffer->Change();
		bChange = false;
		prev = position;
	}
}

void Camera::Render()
{
	viewBuffer->Render();
}

void Camera::Insfactor()
{
	__super::Insfactor();
}
void Camera::Shake(float power, float x, float y)
{
	shake = Vector3(x, y, power);
	bShake = true;
}

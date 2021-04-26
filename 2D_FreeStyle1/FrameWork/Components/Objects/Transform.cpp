#include "stdafx.h"
#include "Transform.h"
#include "./Renderer/Buffers/CBuffer.h"
#include "./GameObjects/GameObject.h"
Transform::Transform()
	: Component(L"Transform")
	, position(0,0,0)
	, scale(1,1,1)
	, angle(0)
	, pivot(0,0)
	, pParent(NULL)
	, pFirstChild(NULL)
	, pNextSibling(NULL)
{
	D3DXMatrixIdentity(&world);
	worldDesc.data = world;
	worldBuffer = new CBuffer(&worldDesc, sizeof(CB_World));
	worldBuffer->Slot(1);
}


Transform::~Transform()
{
}

void Transform::Update()
{
	if (bActive && bChange)
	{
		D3DXMATRIX S, R, T;

		D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
		D3DXMatrixRotationZ(&R, angle);

		D3DXVECTOR3 vec;
		vec.x = position.x + scale.x / 2 - scale.x * pivot.x;
		vec.y = position.y + scale.y / 2 - scale.y * pivot.y;
		vec.z = 0.0f;
		D3DXMatrixTranslation(
			&T,
			vec.x,
			vec.y,
			vec.z);

		world = S * R * T;

		if (pParent != NULL)
			world = world * pParent->World();
		if (pFirstChild != NULL)
			pFirstChild->bChange = true;
		if (pNextSibling != NULL)
			pNextSibling->bChange = true;

		D3DXMatrixTranspose(&worldDesc.data, &world);
		worldBuffer->Change();
		bChange = false;
	}
}

void Transform::Render()
{
	if (bActive)
		worldBuffer->Render();
}

void Transform::Insfactor()
{
	ImGui::InputFloat3("Position", position, 0);
	ImGui::InputFloat("Rotation", &angle, 0.0f, 0.0f, 0);
	ImGui::InputFloat3("Scale", scale, 0);

	ImGui::InputFloat2("Pivot", pivot, 1);
}

void Transform::Pivot(D3DXVECTOR2 vec)
{
	pivot = vec;
	bChange = true;
}

void Transform::Position(float x, float y)
{
	position = Vector3(x, y, 0.0f);
	bChange = true;
}

void Transform::Position(D3DXVECTOR3 vec)
{
	position = vec;
	bChange = true;
}

D3DXVECTOR3 Transform::Position(Space s)
{
	if (s == Space::Local)
		return position;
	else
		return D3DXVECTOR3(world._41, world._42, world._43);
}

void Transform::Scale(float x, float y)
{
	scale = Vector3(x, y, 1.0f);
	bChange = true;
}

void Transform::Scale(D3DXVECTOR3 vec)
{
	scale = vec;
	bChange = true;
}

D3DXVECTOR3 Transform::Scale(Space s)
{
	if (s == Space::Local)
		return scale;
	else 
	{
		D3DXVECTOR3 x, y, z;
		memcpy(x, &world._11, sizeof(float) * 3);
		memcpy(y, &world._21, sizeof(float) * 3);
		memcpy(z, &world._31, sizeof(float) * 3);
		return D3DXVECTOR3
		(
			D3DXVec3Length(&x),
			D3DXVec3Length(&y),
			D3DXVec3Length(&z)
		);
	}
}

void Transform::Rotation(float radian)
{
	angle = radian;
	bChange = true;
}

void Transform::RotationDegree(float degree)
{
	angle = Math::ToRadian(degree);
	bChange = true;
}

void Transform::AddChild(GameObject * newChild)
{
	Transform * child = newChild->transform;

	if (child->pParent == this) return;

	child->RelaseParent();

	Transform* pChild = this->pFirstChild;
	if (pChild == NULL)
	{
		this->pFirstChild = child;
		child->pParent = this;
	}
	else
	{
		while (pChild != NULL)
		{
			if (pChild->pNextSibling == NULL)
			{
				pChild->pNextSibling = child;
				child->pParent = this;
				break;
			}
			pChild = pChild->pNextSibling;
		}
	}
}

void Transform::AttachTo(GameObject * newParent)
{
	newParent->transform->AddChild(this->gameObject);
}

void Transform::RelaseParent()
{
}

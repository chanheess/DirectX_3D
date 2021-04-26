#include "stdafx.h"
#include "Goaldae.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"

Goaldae::Goaldae()
	: GameObject(L"Goaldae")
{
	AddComponent(new Sprite(L"backgoaldae.png"));
	transform->Position(638, 630);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
	transform->Scale(35, 40);
}


Goaldae::~Goaldae()
{
}

void Goaldae::Update()
{
	__super::Update();
}

void Goaldae::Render()
{
	__super::Render();
}

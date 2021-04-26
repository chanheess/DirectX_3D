#include "stdafx.h"
#include "Goaldae2.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"

Goaldae2::Goaldae2()
	: GameObject(L"Goaldae2")
{
	AddComponent(new Sprite(L"frontgoaldae.png"));
	transform->Position(638, 630);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
	transform->Scale(35, 40);
}


Goaldae2::~Goaldae2()
{
}

void Goaldae2::Update()
{
	__super::Update();
}

void Goaldae2::Render()
{
	__super::Render();
}

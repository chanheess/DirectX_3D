#include "stdafx.h"
#include "p1color.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
#include "./Components/Bounding/BBox.h"

p1color::p1color()
	:GameObject(L"p1color")
{
	AddComponent(new BBox);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
	transform->Scale(Vector3(80, 24, 0));
	AddComponent(new Sprite(L"1p.png", 0, 0, 105, 40));
}


p1color::~p1color()
{
}

void p1color::Update()
{
	__super::Update();

}

void p1color::Render()
{
	__super::Render();
}


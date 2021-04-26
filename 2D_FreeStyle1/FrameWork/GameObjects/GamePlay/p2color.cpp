#include "stdafx.h"
#include "p2color.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
#include "./Components/Bounding/BBox.h"

p2color::p2color()
	:GameObject(L"p2color")
{
	AddComponent(new BBox);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
	transform->Scale(Vector3(80, 24, 0));
	AddComponent(new Sprite(L"2p.png", 0, 0, 105, 40));
}


p2color::~p2color()
{
}

void p2color::Update()
{
	__super::Update();

}

void p2color::Render()
{
	__super::Render();
}


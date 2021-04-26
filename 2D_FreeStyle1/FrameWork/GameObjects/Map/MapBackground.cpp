#include "stdafx.h"
#include "MapBackground.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"

MapBackground::MapBackground()
	:GameObject(L"MapBackground")
{
	//AddComponent(new Sprite(L"stage1.png"));
	AddComponent(new Sprite(L"Map2.png"));
	transform->Position(-260, 0);
	transform->Scale(1800, 800);
}


MapBackground::~MapBackground()
{
}

void MapBackground::Update()
{
	__super::Update();
}

void MapBackground::Render()
{
	__super::Render();
}

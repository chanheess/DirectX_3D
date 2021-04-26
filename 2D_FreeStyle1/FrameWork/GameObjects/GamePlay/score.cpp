#include "stdafx.h"
#include "score.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
#include "./Components/Bounding/BBox.h"

score::score()
	:GameObject(L"score")
{
	AddComponent(new BBox);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
}


score::~score()
{
}

void score::Update()
{
	__super::Update();

}

void score::Render()
{
	__super::Render();
}


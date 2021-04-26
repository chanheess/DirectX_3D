#include "stdafx.h"
#include "ObjectX.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
#include "./Components/Bounding/BBox.h"

ObjectX::ObjectX()
	:GameObject(L"object")
{
	AddComponent(new BBox);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
	transform->Position(638, 315);
	transform->Scale(Vector3(30, 9, 0));
	AddComponent(new Sprite(L"Shadow.png", 0, 0, 105, 40));
}


ObjectX::~ObjectX()
{
}

void ObjectX::Update()
{
	__super::Update();

}

void ObjectX::Render()
{
	__super::Render();
}

void ObjectX::Pos(Vector3 pos, int option)
{
	pos2 = pos;
	switch (option)
	{
		case 1:	//공 날아갈때
			break;
		case 2:	//캐릭터
			pos2.y = pos.y - 75.0f;
			transform->Scale(Vector3(80, 24, 0));
			break;
		case 3:	//공 튕겼을때
			pos2 = pos;
			pos2.y = pos.y - 10;
			break;
		case 4:	//캐릭터
			pos2.y = pos.y - 95.0f;
			transform->Scale(Vector3(80, 24, 0));
			break;
	}
	transform->Position(pos2);
}


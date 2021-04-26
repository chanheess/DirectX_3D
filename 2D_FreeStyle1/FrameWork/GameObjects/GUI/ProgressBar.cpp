#include "stdafx.h"
#include "ProgressBar.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
ProgressBar::ProgressBar(wstring name)
	:GameObject(name)
{
	back = new GameObject(L"Back");
	back->transform->AttachTo(this);
	front = new GameObject(L"Front");
	front->transform->AttachTo(this);

	back->AddComponent(new Sprite(L"DefaultDiffuse.png"));
	front->AddComponent(new Sprite(L"DefaultDiffuse.png"));
	totalGauge = nowGauge = 100;
//	back->GetComponent<Sprite>()->DiffuseColor(0xFF000000);
//	front->GetComponent<Sprite>()->DiffuseColor(0xFFFF0000);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Update()
{
	GameObject::Update();
	back->transform->Scale(totalGauge, 20);
	front->transform->Scale(nowGauge, 20);
}

void ProgressBar::Render()
{
	GameObject::Render();
}

void ProgressBar::SetGauge(float gauge)
{
	nowGauge = gauge;
}

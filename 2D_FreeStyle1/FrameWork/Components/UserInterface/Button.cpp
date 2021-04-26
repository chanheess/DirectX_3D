#include "stdafx.h"
#include "Button.h"
#include "./Components/Bounding/BBox.h"
#include "./Components/Animation/Sprite.h"

#include "./Utilities/Intersect.h"
Button::Button()
	:GameObject(L"Button")
{
	tag = Tag::UserInterface;
	AddComponent(new BBox);
	AddComponent(new Sprite(L"Default.png"));
	after = new Sprite(L"Default.png");
	after->Diffuse(0xFFFF0000);
	bClick = false;
}


Button::~Button()
{
	SAFE_DELETE(after);
}

void Button::Ready()
{
	__super::Ready();

}

void Button::Update()
{
	__super::Update();

	
}

void Button::Render()
{
	__super::Render();



}

void Button::Insfactor()
{
}
void Button::AddOneClickEvent(function<void(void)> fp)
{
	events.push_back(fp);
}

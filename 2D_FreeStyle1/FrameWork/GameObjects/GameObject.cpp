#include "stdafx.h"
#include "GameObject.h"
#include "./Utilities/String.h"
#include "./Utilities/Intersect.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Sprite.h"
#include "./Components/Bounding/Gizmo.h"
#include "./Components/Bounding/BBox.h"
//---------------------------------------------------------------------
//	Game Object
//---------------------------------------------------------------------
GameObject::GameObject(wstring name)
	: name(name)
	, bActive(true)
	, sprite(NULL)
	, tag(Tag::Empty)
{
	transform = new Transform();
	transform->gameObject = this;
}

GameObject::~GameObject()
{
	SAFE_DELETE(transform);
	for (Pair comp : components)
		SAFE_DELETE(comp.second);
	SAFE_DELETE(sprite);
}

void GameObject::Ready()
{
	transform->Ready();
	for (Pair comp : components)
		comp.second->Ready();
	if (sprite != NULL)
		sprite->Ready();

	if (transform->pNextSibling)
		transform->pNextSibling->gameObject->Ready();

	if (transform->pFirstChild)
		transform->pFirstChild->gameObject->Ready();

}

void GameObject::Update()
{
	if (bActive == false) return;

	transform->Update();
	for (Pair comp : components)
		comp.second->Update();
	if (sprite != NULL)
		sprite->Update();

	if (transform->pNextSibling)
		transform->pNextSibling->gameObject->Update();

	if (transform->pFirstChild)
		transform->pFirstChild->gameObject->Update();
}

void GameObject::Render()
{
	if (bActive == false) return;
	transform->Render();
	for (Pair comp : components)
		comp.second->Render();
	if (sprite != NULL)
		sprite->Render();

	if (transform->pNextSibling)
		transform->pNextSibling->gameObject->Render();

	if (transform->pFirstChild)
		transform->pFirstChild->gameObject->Render();
}

void GameObject::Insfactor()
{
	ImGui::Checkbox(String::ToString(name).c_str(), &bActive);
	
	transform->Insfactor();
	for (Pair comp : components)
		comp.second->Insfactor();
	if (sprite != NULL)
		sprite->Insfactor();
}

void GameObject::Hierarchy()
{
	string str = String::ToString(name).c_str();
	if(ImGui::TreeNode(str.c_str()))
	{
		if (transform->pFirstChild)
			transform->pFirstChild->gameObject->Hierarchy();

		ImGui::TreePop();
	}
	if (transform->pNextSibling)
		transform->pNextSibling->gameObject->Hierarchy();
}

void GameObject::AddComponent(Component * comp)
{
	comp->SetGameObject(this);
	if (typeid(*comp).name() == (typeid(Sprite).name()))
	{
		sprite = dynamic_cast<Sprite*>(comp);
		return;
	}

	if (components.count(typeid(*comp).name()) > 0)
	{
		MessageBoxA(NULL, "Already Input Component",
			"Warning", MB_OK);
	}
	else
	{
		components[typeid(*comp).name()] = comp;
	}
}
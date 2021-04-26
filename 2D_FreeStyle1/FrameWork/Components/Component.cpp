#include "stdafx.h"
#include "Component.h"
#include "./Utilities/String.h"
Component::Component(wstring name)
	:name(name), bActive(true), bChange(false)
{
}

Component::~Component()
{
}

void Component::Ready()
{
}

void Component::Update()
{
}

void Component::Render()
{
}

void Component::Insfactor()
{
	ImGui::Separator();
	ImGui::Checkbox(String::ToString(name).c_str(), &bActive);

}

#include "stdafx.h"
#include "Input.h"

Input* Input::instance = NULL;

Input * Input::Get()
{
	if (instance == NULL)
		instance = new Input();
	return instance;
}

void Input::Delete()
{
	SAFE_DELETE(instance);
}

bool Input::Key(int key)
{
	if(GetAsyncKeyState(key) & 0x8000)
		return true;

	return false;
}

bool Input::KeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (down[key] == false)
		{
			down.set(key, true);
			return true;
		}	
	}
	else
	{
		down.set(key, false);
	}
	return false;
}

bool Input::KeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		up.set(key, true);
	}
	else
	{
		if (up[key] == true)
		{
			up.set(key, false);
			return true;
		}
	}
	return false;
}

bool Input::Toggle(int key)
{
	if (GetKeyState(key) & 0x0001)
		return true;
	return false;
}

void Input::Update()
{
	sreenMouse.x = mouse.x - MainWindowPos.x;
	sreenMouse.y = 
		height - (mouse.y - MainWindowPos.y);

	sreenMouse.x = Math::Clamp(sreenMouse.x, 0.0f, width);
	sreenMouse.y = Math::Clamp(sreenMouse.y, 0.0f, height);
	sreenMouse.z = 0.0f;
}

Input::Input()
{
	for (int i = 0; i < 256; i++)
	{
		down.set(i, false);
		up.set(i, false);
	}
	D2DDesc desc;
	D2D::GetDesc(&desc);
	width = desc.Width;
	height = desc.Height;
	
	mouse = Vector3(0, 0, 0);
}


Input::~Input()
{
}

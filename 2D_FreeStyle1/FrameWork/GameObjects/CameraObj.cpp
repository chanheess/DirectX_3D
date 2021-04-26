#include "stdafx.h"
#include "CameraObj.h"
#include "Components/Objects/Camera.h"

CameraObj::CameraObj(wstring name)
	:GameObject(name)
{
	AddComponent(new Camera());
}


CameraObj::~CameraObj()
{
}

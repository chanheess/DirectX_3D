#include "stdafx.h"
#include "Scenes.h"

Scenes* Scenes::instance = NULL;

Scenes::Scenes(): nowScene(NULL), releaseScene(NULL){}
Scenes::~Scenes(){}

Scenes * Scenes::Get()
{
	if (instance == NULL)
		instance = new Scenes;
	return instance;
}

void Scenes::Delete()
{
	SAFE_DELETE(instance);
}

void Scenes::Update()
{
	if (releaseScene != NULL)
	{
		releaseScene->Ready();
		releaseScene = NULL;
	}
	if (nowScene != NULL)
		nowScene->Scene_Update();
}

void Scenes::Render()
{
	if (nowScene != NULL)
		nowScene->Scene_Render();
}

void Scenes::AddScene(wstring key, Scene * scene)
{
	assert(sceneMap.count(key) < 1);
	scene->Ready();
	sceneMap[key] = scene;
}

void Scenes::ChangScene(wstring key)
{
	assert(sceneMap.count(key) > 0);

	if (nowScene == NULL)
		nowScene = sceneMap[key];
	else
	{
		releaseScene = nowScene;
		nowScene = sceneMap[key];
	}
	nowScene->Scene_Ready();
}

Scene * Scenes::GetScene(wstring key)
{
	if (sceneMap.count(key) < 1)
		assert(false);

	return sceneMap[key];
}

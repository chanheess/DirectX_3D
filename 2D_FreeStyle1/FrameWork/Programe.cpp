#include "stdafx.h"
#include "Programe.h"
#include "./Widgets/WMenuBar.h"
#include "./Systems/RenderTarget.h"
#include "./Scenes/Scenes.h"
#include "./Scenes/PlayScene.h"
#include "./Components./Etc/Audio.h"
Vector2 MainWindowPos;
Programe::Programe()
{
	srand(time(NULL));
}
Programe::~Programe()
{
}
void Programe::Initialize()
{
	//´ÙÇü¼º 
	Gui::Get()->AddWidget(new WMenuBar());
	Scenes::Get()->AddScene(L"Test", new PlayScene);
	Scenes::Get()->ChangScene(L"Test");
	audio = new Audio();
	audio->Set(L"backmusic.wav", true, true);
	audio->Play();
}

void Programe::Release()
{
	Scenes::Delete();
}

void Programe::Update()
{
	Scenes::Get()->Update();
}

void Programe::PreRender()
{
	//Scenes::Get()->Render();
}

void Programe::Render()
{
	Scenes::Get()->Render();
}

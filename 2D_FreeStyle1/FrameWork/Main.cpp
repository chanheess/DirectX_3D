#include "stdafx.h"
#include "Programe.h"

int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow
)
{
	D2DDesc desc;
	desc.AppName = L"프리스타일";
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Width = 1280;
	desc.Height = 800;
	D2D::SetDesc(desc);


	Programe* window = new Programe();
	window->Create(hInstance);
	
	WPARAM wParam = window->Run();

	delete window;

	return wParam;
}

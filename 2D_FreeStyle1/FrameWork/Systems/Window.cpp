#include "stdafx.h"
#include "Window.h"
#include "./Systems/RenderTarget.h"
#include "./Components/Etc/Audio.h"

Window::Window()
{
}


Window::~Window()
{
}

//메세지 처리 int(정수 - 4byte), LONG(정수 - 8byte)
//CallBack -> 
//동기 함수
//비동기 함수(CALLBACK)
LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (Gui::MsgProc(handle, message, wParam, lParam))
		return true;
	if (message == WM_MOUSEMOVE)
	{
		D3DXVECTOR3 mouse;
		Input::Get()->Position(&mouse);
		mouse.z = 0.0f;
		mouse.x = static_cast<float>(LOWORD(lParam));
		mouse.y = static_cast<float>(HIWORD(lParam));
		Input::Get()->Position(mouse);
	}
	else if (message == WM_MOUSEWHEEL)
	{
		short delta = HIWORD(wParam);
		delta = delta / WHEEL_DELTA;//WHELL_DELTA /120

		Vector3 pos;
		Input::Get()->Position(&pos);
		pos.z = delta;
		Input::Get()->Position(pos);
	}
	else if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(handle, message, wParam, lParam);
}

//윈도 
void Window::Create(HINSTANCE hIns)
{
	D2DDesc desc;
	D2D::GetDesc(&desc);

	//1.ResterWindow 
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0; //메모리 관련
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hInstance = hIns;
	wndClass.lpszClassName = desc.AppName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	//WORD -> 2byte 데이터 
	WORD word = RegisterClassEx(&wndClass);//클래스 등록

	assert(word != 0); 

	//화면에 창을 띄우기 위해 창 생성
	desc.Handle = CreateWindowEx
	(
		WS_EX_APPWINDOW,
		desc.AppName.c_str(), //내가 등록한 창의 옵션
		desc.AppName.c_str(), //타이틀
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,	//x
		CW_USEDEFAULT,	//y
		CW_USEDEFAULT,	//width
		CW_USEDEFAULT,	//height
		NULL,
		(HMENU)NULL,
		hIns,
		NULL
	);


	RECT rc = { 0,0, 1280 ,800 };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - 1280) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - 800) / 2;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);	//윈도우 재정의

	MoveWindow(
		desc.Handle,
		centerX, centerY,
		rc.right - rc.left,
		rc.bottom - rc.top, true
	);

	D2D::SetDesc(desc);
	assert(desc.Handle != NULL);//참 , 거짓
	ShowWindow(desc.Handle, SW_SHOWNORMAL); //창을 출력 하자 
	SetForegroundWindow(desc.Handle);
	SetFocus(desc.Handle);
}

WPARAM Window::Run()//동작
{
	MSG msg;
	D2D::Get();
	Gui::Create();
	Audio::Create();
	DirectWrite::Create();
	Initialize();
	rtt = new RenderTarget();
	while (true)//게임용 루프
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Update
			{
				TIMEMANAGER->SetFrame(FIXFRAME);
				Gui::Get()->Update();
				Input::Get()->Update();
				Update();
				Buffers::Update();
			}
			rtt->Set(0xFF404040);
			{
				DirectWrite::GetDc()->BeginDraw();
				{
					PreRender();
				}
				DirectWrite::GetDc()->EndDraw();
			}
			D2D::Get()->Clear(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
			{
				TIMEMANAGER->render();
				Render();	
				Gui::Get()->Render();
			}
			D2D::Get()->Present();
		}
	}
	Release();
	Audio::Delete();
	DirectWrite::Delete();
	Buffers::Delete();
	Input::Delete();
	TIMEMANAGER->ReleaseSingleton();
	SAFE_DELETE(rtt);
	D2D::Delete();
	return msg.wParam;
}

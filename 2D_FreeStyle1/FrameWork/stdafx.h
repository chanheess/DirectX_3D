#pragma once
//stdafx.h
//Windows
#include <Windows.h>
#include <assert.h>
#include <time.h>

//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//C 관련
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//STD 
#include <string>
#include <vector>
#include <map>
#include <functional>
using namespace std;

//DirectX SDK
#include <dxgi1_2.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h> 
#include <d3d11shader.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "D3DX10.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

//D2D1 + DirectWrite
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//ImGui
#include <ImGui_New/imgui.h>
#include <ImGui_New/imGui_impl_dx11.h>
#include <ImGui_New/imGui_impl_win32.h>
#pragma comment (lib, "ImGui_New/ImGui.lib")

//fMod
#include "../_Libraries/FMOD/inc/fmod.hpp"
#pragma comment(lib, "../_libraries/FMOD/lib/fmodex_vc.lib")


//Systems
#include "./Systems/D2D.h"
#include "./Systems/Input.h"
#include "./Systems/DirectWrite.h"
#include "./Systems/Gui.h"
#include "./Systems/memoryPool.h"

//Renderer
#include "./Renderer/Buffers/Buffer.h"
#include "./Renderer/Shader.h"
#include "./Renderer/Vertex.h"

//Bounding
#include "./Components/Bounding/ILine.h"

//Utilities
#include "./Utilities/Math.h"
#include "./Utilities/Xml.h"

//	Macro
#define SAFE_RELEASE(p) {if(p){	p->Release(); p = NULL;}}
#define SAFE_DELETE(p) {if(p) {delete p; p = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] p; p = NULL;}}

//Typedef
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX Matrix;

#define TRIANGLELIST D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST 
#define LINELIST D3D11_PRIMITIVE_TOPOLOGY_LINELIST

//Time
#include "./Systems/TimeManager.h"
#define TIMEMANAGER TimeManager::GetInstance()
#define g_ETime TimeManager::GetInstance()->GetElapsedTime()
#define FIXFRAME 100
//Player
#include "GameObjects/GamePlay/Player.h"
#include "GameObjects/GamePlay/Player2.h"
#define PLAYER Player::GetInstance()
#define PLAYER2 Player2::GetInstance()


extern Vector2 MainWindowPos;

//함수 클래스 외부에서 작성한 변수를 전역 변수 
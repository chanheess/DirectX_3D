#include "stdafx.h"
#include "Scene.h"
#include "./GameObjects/GameObject.h"
#include "./GameObjects/CameraObj.h"
#include "./Components/Objects/Camera.h"
#include "./Components/Objects/Transform.h"
#include "./Utilities/Path.h"
ID3D11BlendState * Scene::alphBlend = NULL;
ID3D11RasterizerState* Scene::cullmode = NULL;
UINT Scene::refCount = 0;

Scene::Scene()
{
	root = new GameObject(L"World");
	rootTransform = root->transform;

	mainCameraObj = new CameraObj(L"MainCamera");
	mainCamera = mainCameraObj->GetComponent<Camera>();
	rootTransform->AddChild(mainCameraObj);

	refCount++;
	{
		if (cullmode != NULL) return;
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.CullMode = D3D11_CULL_NONE;
		D2D::GetDevice()->CreateRasterizerState
		(
			&desc, &cullmode
		);
		D2D::GetDC()->RSSetState(cullmode);
	}
	
	{
		if (alphBlend != NULL) return;
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//R 1, 0, 0 G 0, 1, 0
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D2D::GetDevice()->CreateBlendState(&desc, &alphBlend);
		D2D::GetDC()->OMSetBlendState(alphBlend, 0, 0xFFFFFFFF);
	}
};
Scene::~Scene()
{
	SAFE_DELETE(mainCamera);
	refCount--;
	if (refCount == 0)
	{
		SAFE_RELEASE(alphBlend);
	}
	SAFE_DELETE(root);
}
void Scene::Scene_Ready()
{
	root->Ready();
	Ready();
}
void Scene::Scene_Update()
{
	root->Update();
	Update();
}
void Scene::Scene_Render()
{
	root->Render();
	Render();
}

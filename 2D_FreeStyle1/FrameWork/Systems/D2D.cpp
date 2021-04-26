#include "stdafx.h"
#include "D2D.h"

D2D* D2D::instance = NULL;
ID3D11Device* D2D::device = NULL;
ID3D11DeviceContext* D2D::deviceContext = NULL;
D2DDesc D2D::desc;


D2D::D2D()
{
	this->SetGpuInfo();
	this->CreateDeviceAndSwapChain();
	this->CreateBackBuffer();
	this->CreateViewport();
}


D2D::~D2D()
{
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(RTV);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}

D2D * D2D::Get()
{
	if (instance == NULL)
		instance = new D2D();
	return instance;
}

void D2D::Delete()
{
	SAFE_DELETE(instance);
}

void D2D::SetGpuInfo()
{
	HRESULT hr;

	IDXGIFactory* factory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter;
	hr = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(hr));

	IDXGIOutput* adapterOutput;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(hr));

	UINT modeCount;
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&modeCount,
		NULL
	);
	assert(SUCCEEDED(hr));

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[modeCount];
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&modeCount,
		displayModeList
	);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < modeCount;i++)
	{
		bool isCheck = true;
		isCheck &= displayModeList[i].Width == desc.Width;
		isCheck &= displayModeList[i].Height == desc.Height;
		if (isCheck)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}
	DXGI_ADAPTER_DESC adapterDesc;
	hr = adapter->GetDesc(&adapterDesc);
	assert(SUCCEEDED(hr));

	gpuMemorySize = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	gpuDescription = adapterDesc.Description;

	SAFE_DELETE_ARRAY(displayModeList);

	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
}

void D2D::CreateDeviceAndSwapChain()
{
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	DXGI_SWAP_CHAIN_DESC swapChindesc;
	ZeroMemory(&swapChindesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	bufferDesc.Width = 0;
	bufferDesc.Height = 0;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChindesc.BufferDesc = bufferDesc;
	swapChindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChindesc.SampleDesc.Count = 1;
	swapChindesc.SampleDesc.Quality = 0;
	swapChindesc.BufferCount = 1;
	swapChindesc.OutputWindow = desc.Handle;
	swapChindesc.Windowed = !desc.bFullScreen;
	swapChindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChindesc.Flags = 0;
	if (desc.bVsync == true)
	{
		bufferDesc.RefreshRate.Numerator = numerator;
		bufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		bufferDesc.RefreshRate.Numerator = 0;
		bufferDesc.RefreshRate.Denominator = 1;
	}
	

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels[7] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};


	//성공시(S_OK), 실패 (E_FAIL, E_
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapChindesc, 
		&swapChain,
		&device,
		NULL,
		&deviceContext
	);
	assert(SUCCEEDED(hr) && "Not Initialize Device");
}

void D2D::CreateBackBuffer()
{
	HRESULT hr;
	ID3D11Texture2D* backbuffer;
	hr = swapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D), (void**)&backbuffer);

	assert(SUCCEEDED(hr) && "Not Get SwapChain Data");

	hr = device->CreateRenderTargetView(backbuffer, NULL, &RTV);
	assert(SUCCEEDED(hr) && "Not Create RTV");

	
}

void D2D::CreateViewport()
{
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = desc.Width;
	viewport.Height = desc.Height;

	deviceContext->RSSetViewports(1, &viewport);
}

void D2D::Clear(D3DXCOLOR color ,ID3D11RenderTargetView* rtv )
{
	if (rtv == NULL)
		rtv = RTV;

	deviceContext->OMSetRenderTargets(1, &rtv, NULL);
	deviceContext->ClearRenderTargetView(rtv, color);
}

void D2D::Present()
{
	swapChain->Present(0, 0);
}

void D2D::SetDesc(D2DDesc & val)
{
	desc = val;
}

void D2D::GetDesc(D2DDesc * val)
{
	*val = desc;
}

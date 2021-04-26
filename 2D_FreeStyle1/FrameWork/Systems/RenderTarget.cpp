#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT wdith, UINT height)
{
	//이미지 크기 설정
	{
		D2DDesc desc;
		D2D::GetDesc(&desc);
		this->wdith = (wdith < 1) ? (UINT)desc.Width : wdith;
		this->height = (height < 1) ? (UINT)desc.Height : height;
	}

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	//Create Texture - RTV용(빈 텍스쳐)
	{
		textureDesc.Width = this->wdith;
		textureDesc.Height = this->height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags =
			D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_RENDER_TARGET;
		HRESULT hr = D2D::GetDevice()->CreateTexture2D(
			&textureDesc, NULL, &rtvTexture
		);
		assert(SUCCEEDED(hr));
	}
	//Create RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		HRESULT hr = D2D::GetDevice()->CreateRenderTargetView(
			rtvTexture, &desc, &rtv
		);
		assert(SUCCEEDED(hr));
	}
	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = textureDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;

		HRESULT hr = D2D::GetDevice()->CreateShaderResourceView(
			rtvTexture, &desc, &srv
		);
		assert(SUCCEEDED(hr));
	}
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(rtvTexture);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtv);
}

void RenderTarget::Set(D3DXCOLOR clear)
{
	D2D::Get()->Clear(clear, rtv);
}

void RenderTarget::SaveRtvTexture(wstring saveFile)
{
	wstring file = L"../_Resources/Textures/" + saveFile;
	HRESULT hr = D3DX11SaveTextureToFile
	(
		D2D::GetDC(),
		rtvTexture,
		D3DX11_IFF_PNG,
		file.c_str()
	);
	assert(SUCCEEDED(hr));
}

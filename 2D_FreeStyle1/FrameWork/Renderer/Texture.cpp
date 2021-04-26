#include "stdafx.h"
#include "Texture.h"
#include "./Utilities/String.h"
ID3D11SamplerState* Texture::WrapSampler = NULL;

Texture::Texture(wstring fileName)
{
	int idx = fileName.find_last_of('/');
	if (idx != -1)
	{
		fileName = fileName.substr(idx + 1, fileName.length());
	}
	sprintf_s(str, String::ToString(fileName).c_str());
	file = L"../_Resources/Textures/" + fileName;
	SRV = Textures::Load(file);

	if (WrapSampler != NULL) return;
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
	//WRAP -> uv 0~1 사이를 벗어났을때 0~1사이로 계산
	//Miror -> 
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	D2D::GetDevice()->CreateSamplerState
	(
		&desc, &WrapSampler
	);
}

Texture::~Texture()
{
	Textures::Remove(file);
}
void Texture::Render()
{
	D2D::GetDC()->PSSetSamplers(0, 1, &WrapSampler);
	D2D::GetDC()->PSSetShaderResources(0, 1, &SRV);
}
void Texture::Insfactor()
{
	ImGui::Image(SRV, ImVec2(200, 200));
	ImGui::InputText("", str, 64);
	if (ImGui::Button("Name"))
	{
		file = L"../_Resources/Textures/"
			+ String::ToWString(str);
		SRV = Textures::Load(file);
	}
}
//---------------------------------------------------------------------
//	Textures
//---------------------------------------------------------------------

map<wstring, Textures::TextureDesc> Textures::TextureMap;

ID3D11ShaderResourceView * Textures::Load(wstring file)
{
	//file 파일 경로 
	if (TextureMap.count(file) > 0)
	{
		TextureMap[file].RefCount++;

		return TextureMap[file].SRV;
	}//있으면

	HRESULT hr;
	ID3D11ShaderResourceView* srv;
	//D3DX11/Create/Shader/Resource/View/From/File
	hr = D3DX11CreateShaderResourceViewFromFile(
		D2D::GetDevice(), file.c_str(), NULL, NULL, &srv, NULL
	);

	TextureDesc desc;
	desc.RefCount++;
	desc.SRV = srv;

	TextureMap[file] = desc;
	return desc.SRV;
}

void Textures::Remove(wstring file)
{
	UINT count = TextureMap.count(file);
	assert(count > 0);

	TextureMap[file].RefCount--;
	if (TextureMap[file].RefCount < 1)
	{
		SAFE_RELEASE(TextureMap[file].SRV);

		TextureMap.erase(file);
	}
}

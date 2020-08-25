#pragma once

class Texture
{
public:
	friend class Textures;

public:
	void SaveFile(wstring file);
	static void SaveFile(wstring file, ID3D11Texture2D* src);

	D3D11_TEXTURE2D_DESC ReadPixel(DXGI_FORMAT readFormat, vector<Color>* pixels);
	static D3D11_TEXTURE2D_DESC ReadPixel(ID3D11Texture2D* src, DXGI_FORMAT readFormat, vector<Color>* pixels);

public:
	Texture(wstring file, D3DX11_IMAGE_LOAD_INFO* loadInfo = NULL);
	~Texture();
	
	operator ID3D11ShaderResourceView*(){ return view; }


	wstring GetFile() { return file; }

	UINT GetWidth() { return metaData.width; }
	UINT GetHeight() { return metaData.height; }

	void GetImageInfo(DirectX::TexMetadata* data)
	{
		*data = metaData;
	}

	ID3D11ShaderResourceView* SRV() { return view; }
	ID3D11Texture2D* GetTexture();

public:	//height
	void ReadPixel(DXGI_FORMAT readFormat, float* heightColors);
	void CreateWritingTexture(DXGI_FORMAT readFormat, UINT num);
	void CreateSRVTexture(DXGI_FORMAT readFormat, UINT num);
	void ChangeSRVTexture(DXGI_FORMAT readFormat, ID3D11Texture2D* texture, UINT num);

	ID3D11Texture2D* GetWritingTexture() { return writingTexture; }
	ID3D11Texture2D* GetTextureSRV() { return textureSRV; }

public:	//alpha
	void ReadPixel2(DXGI_FORMAT readFormat, vector<Color>* pixels);

	ID3D11Texture2D* GetWritingTexture2() { return writingTexture2; }
	ID3D11Texture2D* GetTextureSRV2() { return textureSRV2; }

private:
	wstring file;

	DirectX::TexMetadata metaData;
	ID3D11ShaderResourceView* view;

private:	//height
	ID3D11Texture2D* writingTexture;
	ID3D11Texture2D* textureSRV;

private:	//alpha
	ID3D11Texture2D* writingTexture2;
	ID3D11Texture2D* textureSRV2;
};

struct TextureDesc
{
	wstring file;
	UINT width, height;
	DirectX::TexMetadata metaData;
	ID3D11ShaderResourceView* view;

	bool operator==(const TextureDesc& desc)
	{
		bool b = true;
		b &= file == desc.file;
		b &= width == desc.width;
		b &= height == desc.height;

		return b;
	}
};

class Textures
{
public:
	friend class Texture;

public:
	static void Create();
	static void Delete();

private:
	static void Load(Texture* texture, D3DX11_IMAGE_LOAD_INFO* loadInfo = NULL);

private:
	static vector<TextureDesc> descs;
};

class TextureArray
{
public:
	TextureArray(vector<wstring>& names, UINT width = 256, UINT height = 256, UINT mipLevels = 1);
	~TextureArray();

	ID3D11ShaderResourceView* SRV() { return srv; }

private:
	vector<ID3D11Texture2D *> CreateTextures(vector<wstring>& names, UINT width, UINT height, UINT mipLevels);

private:
	ID3D11ShaderResourceView* srv;
};
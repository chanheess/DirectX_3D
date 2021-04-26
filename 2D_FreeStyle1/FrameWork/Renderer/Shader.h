#pragma once

struct ShaderDesc
{
	ID3D10Blob* vsBlob = NULL;
	ID3D10Blob* psBlob = NULL;
	ID3D10Blob* csBlob = NULL;
	ID3D11VertexShader* vertexShader = NULL;
	ID3D11PixelShader* pixelShader = NULL;
	ID3D11ComputeShader * computeShader = NULL;
};
class Shader
{
public:
	friend class Shaders;
public:
	Shader(wstring file, bool bCompute = false);
	~Shader();

	void Render();

	void DiffuseColor(D3DXCOLOR color);

	void CreateInputLayout
	(
		D3D11_INPUT_ELEMENT_DESC* desc,
		UINT count
	);

private:
	//Sprite -> Shader
	struct CB_Material
	{
		D3DXCOLOR DiffuseColor;
	}material;
	class CBuffer * materialBuffer;
private:
	ShaderDesc desc;
	ID3D11InputLayout* layout;
};

class Shaders
{
public:
	friend class Shader;

public:
	static void Delete();

private:
	static void Load(wstring file, ShaderDesc* desc, bool bCompute);
	static void GetErrorMessage(ID3D10Blob* error);

private:
	static map<wstring, ShaderDesc> descMap;
};

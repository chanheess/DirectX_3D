#include "stdafx.h"
#include "Shader.h"
#include "./Renderer/Buffers/CBuffer.h"

//----------------------------------------------------------
//	Shader
//----------------------------------------------------------
Shader::Shader(wstring file, bool bCompute)
	: layout(NULL), materialBuffer(NULL)
{
	desc.computeShader = NULL;
	desc.pixelShader = NULL;
	desc.vertexShader = NULL;

	if(bCompute == false)
	{
		material.DiffuseColor = 0xFFFFFFFF;
		materialBuffer = new CBuffer
		(
			&material,sizeof(CB_Material),BufferType::PS
		);
	}

	wstring fileName = L"../_Resources/Shaders/" + file;
	Shaders::Load(fileName, &desc, bCompute);
}

Shader::~Shader()
{
	SAFE_RELEASE(layout);
}

void Shader::Render()
{
	if (layout == NULL)
	{
		CreateInputLayout(
			VertexTexture::desc, VertexTexture::count);
	}
	if(materialBuffer) materialBuffer->Render();
	if(layout) D2D::GetDC()->IASetInputLayout(layout);
	if(desc.vertexShader) D2D::GetDC()->VSSetShader(desc.vertexShader, 0, 0);
	if(desc.pixelShader) D2D::GetDC()->PSSetShader(desc.pixelShader, 0, 0);
	if(desc.computeShader) D2D::GetDC()->CSSetShader(desc.computeShader, 0, 0);
}

void Shader::DiffuseColor(D3DXCOLOR color)
{
	material.DiffuseColor = color;
	materialBuffer->Change();
}

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
	//InputLayout
	if (this->desc.vertexShader == NULL) return;
	{
		HRESULT hr = D2D::GetDevice()->CreateInputLayout
		(
			desc,
			count,
			this->desc.vsBlob->GetBufferPointer(),
			this->desc.vsBlob->GetBufferSize(),
			&layout
		);
	}
}


//----------------------------------------------------------------------------
//	Shaders
//----------------------------------------------------------------------------

map<wstring, ShaderDesc> Shaders::descMap;
void Shaders::Delete()
{
	for (auto desc : descMap)
	{
		SAFE_RELEASE(desc.second.vsBlob);
		SAFE_RELEASE(desc.second.psBlob);
		SAFE_RELEASE(desc.second.csBlob);
		SAFE_RELEASE(desc.second.vertexShader);
		SAFE_RELEASE(desc.second.pixelShader);
		SAFE_RELEASE(desc.second.computeShader);
	}
}

void Shaders::Load(wstring file, ShaderDesc * desc, bool bCompute)
{
	map<wstring, ShaderDesc>::iterator iter = descMap.find(file);

	if (iter != descMap.end()) // 키값으로 등록된 녀석이 있다
	{
		*desc = iter->second;
	}
	else
	{
		ShaderDesc shaderDesc;

		HRESULT hr;
		ID3D10Blob* error;

		if (bCompute == false)
		{
			hr = D3DX10CompileFromFile
			(
				file.c_str(), NULL, NULL, "VS", "vs_5_0",
				0, 0, NULL, &shaderDesc.vsBlob, &error, NULL
			);
			GetErrorMessage(error);
			assert(SUCCEEDED(hr));

			hr = D3DX10CompileFromFile
			(
				file.c_str(), NULL, NULL, "PS", "ps_5_0",
				0, 0, NULL, &shaderDesc.psBlob, &error, NULL
			);
			GetErrorMessage(error);
			assert(SUCCEEDED(hr));

			hr = D2D::GetDevice()->CreateVertexShader
			(
				shaderDesc.vsBlob->GetBufferPointer(),
				shaderDesc.vsBlob->GetBufferSize(),
				NULL,
				&shaderDesc.vertexShader
			);
			assert(SUCCEEDED(hr));

			hr = D2D::GetDevice()->CreatePixelShader
			(
				shaderDesc.psBlob->GetBufferPointer(),
				shaderDesc.psBlob->GetBufferSize(),
				NULL,
				&shaderDesc.pixelShader
			);
			assert(SUCCEEDED(hr));
		}
		else
		{
			hr = D3DX10CompileFromFile
			(
				file.c_str(), NULL, NULL, "CS", "cs_5_0",
				0, 0, NULL, &shaderDesc.csBlob, &error, NULL
			);
			GetErrorMessage(error);
			assert(SUCCEEDED(hr));

			hr = D2D::GetDevice()->CreateComputeShader
			(
				shaderDesc.csBlob->GetBufferPointer(),
				shaderDesc.csBlob->GetBufferSize(),
				NULL,
				&shaderDesc.computeShader
			);
			assert(SUCCEEDED(hr));
		}
		*desc = shaderDesc;
		descMap[file] = shaderDesc;
	}
}

void Shaders::GetErrorMessage(ID3D10Blob * error)
{
	if (error != NULL)
	{
		MessageBoxA(NULL, (char*)error->GetBufferPointer(),
			"Shader Error", MB_OK);
		SAFE_RELEASE(error);
		assert(false);
	}

}

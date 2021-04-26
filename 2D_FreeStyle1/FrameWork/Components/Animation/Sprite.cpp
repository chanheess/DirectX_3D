#include "stdafx.h"
#include "Sprite.h"
#include "./Renderer/Buffers/VBuffer.h"
#include "./Renderer/Buffers/IBuffer.h"
#include "./Renderer/Texture.h"
#include "./Utilities/String.h"

Sprite::Sprite()
	: Component(L"Sprite")
{
	Initialize(L"", 0, 0, 0, 0);
}

Sprite::Sprite(wstring textureFile)
	:Component(L"Sprite")
{
	Initialize(textureFile, 0, 0, 0, 0);
}

Sprite::Sprite(wstring textureFile, UINT startX, UINT startY)
	: Component(L"Sprite")
{
	Initialize(textureFile, startX, startY, 0, 0);

}

Sprite::Sprite(wstring textureFile, UINT startX, UINT startY, UINT endX, UINT endY)
	:Component(L"Sprite")
{
	Initialize(textureFile, startX, startY, endX, endY);

}

Sprite::~Sprite()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(texture);
	SAFE_DELETE_ARRAY(vertices);
}

void Sprite::Update()
{
	if (bChange && bActive)
	{
		vertices[0].Uv = { offset.x, count.y };
		vertices[1].Uv = offset;
		vertices[2].Uv = { count.x,offset.y };
		vertices[3].Uv = count;
	}
}
void Sprite::Render()
{
	vertexBuffer->Render();
	indexBuffer->Render();
	shader->Render();
	texture->Render();
	D2D::GetDC()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	D2D::GetDC()->DrawIndexed(6, 0, 0);
}

void Sprite::Insfactor()
{
	__super::Insfactor();//바로 윗 부모 클래스 __super
	
	ImGui::InputFloat2("Offset", offset, 1);
	ImGui::InputFloat2("Count", count, 1);

	texture->Insfactor();
}

void Sprite::SaveSprite(Xml::XMLElement * node)
{
	node->SetAttribute("Name", String::ToString(file).c_str());
	node->SetAttribute("Width", width);
	node->SetAttribute("Height", height);
	node->SetAttribute("OffsetX", offset.x);
	node->SetAttribute("OffsetY", offset.y);
	node->SetAttribute("CountX", count.x);
	node->SetAttribute("countY", count.y);
}


void Sprite::Uv(D3DXVECTOR2 vec)
{
	for (int i = 0; i < 4; i++)
		vertices[i].Uv += vec;

	vertexBuffer->Change();
	bChange = true;
}

void Sprite::Initialize(wstring textureFile, float startX, float startY, float endX, float endY)
{
	file = textureFile;

	D3DX11_IMAGE_INFO info;
	if (textureFile.length() > 1)
	{
		texture = new Texture(textureFile);
		textureFile = L"../_Resources/Textures/" + textureFile;
		HRESULT hr = D3DX11GetImageInfoFromFile
		(
			textureFile.c_str(), NULL, &info, NULL
		);
		assert(SUCCEEDED(hr));
	}
	else
	{
		texture = NULL;
		info.Width = 1;
		info.Height = 1;
	}
	//VertexBuffer
	{
		offset.x = (startX > 0) ? startX / (float)info.Width : 0.0f;
		offset.y = (startY > 0) ? startY / (float)info.Height : 0.0f;

		count.x = (endX > 0) ? endX / (float)info.Width : 1.0f;
		count.y = (endY > 0) ? endY / (float)info.Height : 1.0f;

		startX = (startX > 0) ? startX : 0.0f;
		startY = (startY > 0) ? startY : 0.0f;
		endX = (endX > 0) ? endX : (float)info.Width;
		endY = (endY > 0) ? endY : (float)info.Height;

		width = endX - startX;
		height = endY - startY;

		vertices = new VertexTexture[4];
		vertices[0] = VertexTexture(-0.5f, -0.5f, offset.x, count.y);
		vertices[1] = VertexTexture(-0.5f, 0.5f, offset.x, offset.y);
		vertices[2] = VertexTexture(0.5f, 0.5f, count.x, offset.y);
		vertices[3] = VertexTexture(0.5f, -0.5f, count.x, count.y);

		vertexBuffer = new VBuffer
		(
			vertices, sizeof(VertexTexture), 4
		);
	}
	//IndexBuffer
	{
		UINT* indices = new UINT[6]{ 0,1,2,0,2,3 };

		indexBuffer = new IBuffer(indices, 6);

		SAFE_DELETE_ARRAY(indices);
	}
	//Shader
	{
		shader = new Shader(L"Color.hlsl");
	}
}

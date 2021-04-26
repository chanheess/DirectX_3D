#include "stdafx.h"
#include "Buffer.h"

Buffer::Buffer(void * data, UINT byteWidth)
	: data(data), byteWidth(byteWidth),
	buffer(NULL), slot(0), bChange(false)
{
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = byteWidth;
	desc.Usage = D3D11_USAGE_DEFAULT;
}

Buffer::~Buffer()
{
	SAFE_RELEASE(buffer);
}

void Buffer::Update()
{
	if (buffer == NULL) return;
	if (bChange)
	{
		D2D::GetDC()->UpdateSubresource
		(
			buffer, 0, 0, data, 0, 0
		);
	}
}

void Buffer::Render()
{
}

void Buffer::Change()
{
	bChange = true;
}

void Buffer::SetData(void * data)
{
	memcpy(this->data, data, byteWidth);
	Change();
	Update();
}

void Buffer::CreateBuffer()
{
	if (data == NULL)
	{
		D2D::GetDevice()->CreateBuffer
		(
			&desc, NULL, &buffer
		);
	}
	else
	{
		D3D11_SUBRESOURCE_DATA Data;
		Data.pSysMem = data;
		D2D::GetDevice()->CreateBuffer
		(
			&desc, &Data, &buffer
		);
	}
}

///////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////

vector<Buffer*> Buffers::buffers;

void Buffers::Add(Buffer * buffer)
{
	buffer->CreateBuffer();
	buffers.push_back(buffer);
}

void Buffers::Update()
{
	for (Buffer* buffer : buffers)
		buffer->Update();
}

void Buffers::Delete()
{
	for (Buffer* buffer : buffers)
		SAFE_DELETE(buffer);
}

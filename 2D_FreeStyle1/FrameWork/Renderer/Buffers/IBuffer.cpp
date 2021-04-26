#include "stdafx.h"
#include "IBuffer.h"


IBuffer::IBuffer(void * data, UINT count)
	:Buffer(data, sizeof(UINT) * count), count(count)
{
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Buffers::Add(this);
}

IBuffer::~IBuffer()
{
}

void IBuffer::Render()
{
	D2D::GetDC()->IASetIndexBuffer(
		buffer, DXGI_FORMAT_R32_UINT, 0
	);
}

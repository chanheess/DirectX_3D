#include "stdafx.h"
#include "VBuffer.h"


VBuffer::VBuffer(void * data, UINT stride, UINT count)
	: Buffer(data, stride * count),stride(stride)
	, count(count), offset(0)
{
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	Buffers::Add(this);
}

VBuffer::~VBuffer()
{
}

void VBuffer::Render()
{
	D2D::GetDC()->IASetVertexBuffers(
		0, 1, &buffer, &stride, &offset);
}

void VBuffer::Render(ID3D11Buffer * buffer, UINT stride, UINT offset)
{
	ID3D11Buffer* buf[2] = { this->buffer, buffer };
	UINT strides[2] = { this->stride, stride };
	UINT offsets[2] = { this->offset , offset };

	D2D::GetDC()->IASetVertexBuffers(
		0, 2, buf, strides, offsets);

}

void VBuffer::Render(VBuffer * buf)
{
	this->Render(buf->buffer, buf->stride, buf->offset);
}

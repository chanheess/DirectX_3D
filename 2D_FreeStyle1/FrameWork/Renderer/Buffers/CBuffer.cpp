#include "stdafx.h"
#include "CBuffer.h"


CBuffer::CBuffer(void * data, UINT byteWidth, BufferType type)
	:Buffer(data, byteWidth), type(type)
{
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	Buffers::Add(this);
}

CBuffer::~CBuffer()
{

}

void CBuffer::Render()
{
	switch (type)
	{
		case BufferType::VS:
		{
			D2D::GetDC()->VSSetConstantBuffers(slot, 1, &buffer);
			break;
		}
		case BufferType::PS:
		{
			D2D::GetDC()->PSSetConstantBuffers(slot, 1, &buffer);
			break;
		}
	}
}

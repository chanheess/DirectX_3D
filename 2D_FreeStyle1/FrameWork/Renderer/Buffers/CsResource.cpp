#include "stdafx.h"
#include "CsResource.h"


CsResource::CsResource()
	: input(NULL),srv(NULL), output(NULL),uav(NULL),result(NULL)
{
}


CsResource::~CsResource()
{
	SAFE_RELEASE(input);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(output);
	SAFE_RELEASE(uav);
	SAFE_RELEASE(result);

}

void CsResource::Copy(void * data, UINT dataSize)
{
	D2D::GetDC()->CopyResource(result, output);

	D3D11_MAPPED_SUBRESOURCE map;
	D2D::GetDC()->Map(result, 0, D3D11_MAP_READ, 0, &map);
	{
		memcpy(data, map.pData, dataSize);
	}
	D2D::GetDC()->Unmap(result, 0);
}
void CsResource::CreateBuffer()
{
	CreateInput();
	CreateSRV();

	CreateOutput();
	CreateUAV();

	CreateResult();
}
///////////////////////////////////////////////////////
RawBuffer::RawBuffer(void * data, UINT dataSize)
	:CsResource(), inputData(data), byteWidth(dataSize)
{
	CreateBuffer();
}

RawBuffer::~RawBuffer()
{
}

void RawBuffer::CreateInput()
{
	ID3D11Buffer* buffer = NULL;

	//셰이더에게 데이터 보낼때는 버퍼, 텍스쳐 
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	//CS -> 셰이더에게 계산을 넘기 겠다 .
	//구조체 단위 데이터를 넣어서 계산을 하겠다가 
	//단일 데이터 배열 

	desc.ByteWidth = byteWidth;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	//D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS 
	//바이트 주소로 리소스를 사용하겠다.

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = inputData;

	HRESULT hr = D2D::GetDevice()->CreateBuffer(
		&desc, &subResource, &buffer
	);
	input = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateSRV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)input;
	
	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;

	HRESULT hr = D2D::GetDevice()->CreateShaderResourceView(
		buffer, &srvDesc, &srv
	);
}

void RawBuffer::CreateOutput()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = byteWidth;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	HRESULT hr = D2D::GetDevice()->CreateBuffer(
		&desc, NULL, &buffer
	);
	output = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output; //수정

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4;

	HRESULT hr = D2D::GetDevice()->CreateUnorderedAccessView(
		buffer, &uavDesc, &uav
	);
}

void RawBuffer::CreateResult()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	((ID3D11Buffer*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = D2D::GetDevice()->CreateBuffer(
		&desc, NULL, &buffer
	);
	result = (ID3D11Resource*)buffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

StructuredBuffer::StructuredBuffer(void * data, UINT stride, UINT count)
	:CsResource(), inputData(data), stride(stride), count(count)
{
	CreateBuffer();
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::CreateInput()
{
	ID3D11Buffer* buffer = NULL;

	//셰이더에게 데이터 보낼때는 버퍼, 텍스쳐 
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = ByteWidth();
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = inputData;

	HRESULT hr = D2D::GetDevice()->CreateBuffer(
		&desc, &subResource, &buffer
	);
	input = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateSRV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)input;

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;

	HRESULT hr = D2D::GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv);
}

void StructuredBuffer::CreateOutput()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = ByteWidth();
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = stride;
	HRESULT hr = D2D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
	output = (ID3D11Resource*)buffer;
}

void StructuredBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output; //수정

	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;

	HRESULT hr = D2D::GetDevice()->CreateUnorderedAccessView(
		buffer, &uavDesc, &uav
	);
}

void StructuredBuffer::CreateResult()
{
	ID3D11Buffer* buffer = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	((ID3D11Buffer*)output)->GetDesc(&desc);
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = D2D::GetDevice()->CreateBuffer(
		&desc, NULL, &buffer
	);
	result = (ID3D11Resource*)buffer;
}

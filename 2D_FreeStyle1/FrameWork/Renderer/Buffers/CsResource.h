#pragma once

class CsResource
{
public:
	CsResource();
	virtual ~CsResource();

	ID3D11ShaderResourceView* SRV() { return srv; }
	ID3D11UnorderedAccessView* UVA() { return uav; }

	void Copy(void*data, UINT dataSize);


protected:
	virtual void CreateInput() {}
	virtual void CreateSRV() {}
	virtual void CreateOutput() {}
	virtual void CreateUAV() {}
	virtual void CreateResult() {}

	void CreateBuffer();
protected:
	ID3D11Resource * input;
	ID3D11ShaderResourceView* srv;
	
	ID3D11Resource * output; //°á°ú 
	ID3D11UnorderedAccessView* uav;

	ID3D11Resource * result;
};

////////////////////////////////////////////////////////

class RawBuffer : public CsResource
{
public:
	RawBuffer(void* data, UINT dataSize);
	~RawBuffer();

public:
	UINT ByteWidth(){ return byteWidth; }
private:
	void CreateInput() override;
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

	void CreateResult() override;

private:
	void* inputData;
	UINT byteWidth;
};

////////////////////////////////////////////////////////

class StructuredBuffer : public CsResource
{
public:
	StructuredBuffer(void* data, UINT stride, UINT count);
	~StructuredBuffer();

public:
	UINT ByteWidth() { return stride * count; }
private:
	void CreateInput() override;
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

	void CreateResult() override;

private:
	void* inputData;
	UINT stride;
	UINT count;
};
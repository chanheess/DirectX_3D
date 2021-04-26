#pragma once
#include"Buffer.h"
class VBuffer : public Buffer
{
public:
	VBuffer(void * data, UINT stride, UINT count);
	~VBuffer();

	void Render() override;
	void Render(ID3D11Buffer* buffer, UINT strid, UINT offset);
	void Render(VBuffer* buf);

private:
	UINT stride;
	UINT offset;
	UINT count;

};


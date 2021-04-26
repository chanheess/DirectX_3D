#pragma once
#include "Buffer.h"
class IBuffer : public Buffer
{
public:
	IBuffer(void* data, UINT count);
	~IBuffer();

	void Render() override;
private:
	UINT count;
};


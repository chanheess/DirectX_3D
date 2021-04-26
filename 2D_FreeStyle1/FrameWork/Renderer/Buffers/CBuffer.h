#pragma once
#include "Buffer.h"
enum class BufferType
{
	VS, PS
};
class CBuffer : public Buffer
{
public:
	CBuffer(void* data, UINT byteWidth, BufferType type = BufferType::VS);
	~CBuffer();

	void Render() override;

private:
	BufferType type;
};

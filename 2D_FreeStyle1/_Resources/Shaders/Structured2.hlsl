//FileName : Structured.hlsl

struct Data
{
	int i;
	float f;
};

StructuredBuffer<Data> Input;
RWStructuredBuffer<Data> Output;

[numthreads(64, 1, 1)]
void CS(uint GroupIndex : SV_GroupIndex)
{
	Output[GroupIndex].i = Input[GroupIndex].i + 20;
	Output[GroupIndex].f = Input[GroupIndex].f + 1.4f;


}

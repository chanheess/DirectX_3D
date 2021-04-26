//FileName : Structured.hlsl

struct Data
{
	int i;
	float f;
};
struct Data2
{
	int i;
	float f;
	float2 v;
};


StructuredBuffer<Data> Input1 : register(t0);
StructuredBuffer<Data2> Input2 : register(t1);
RWStructuredBuffer<Data2> Output : register(u0);

[numthreads(64, 1, 1)]
void CS(uint GroupIndex : SV_GroupIndex)
{
	Output[GroupIndex].i 
		= Input1[GroupIndex].i + Input2[GroupIndex].i;
	Output[GroupIndex].f 
		= Input1[GroupIndex].f + Input2[GroupIndex].f;
	Output[GroupIndex].v.x = Output[GroupIndex].i;
	Output[GroupIndex].v.y = Output[GroupIndex].f;


}

//FileName : Structured.hlsl

struct Data
{
	uint Number;
	float3 Id;
	uint Data;
};

StructuredBuffer<Data> Input;
RWStructuredBuffer<Data> Output;

[numthreads(16, 1, 1)]
void CS(
	uint3 groupID : SV_GroupID,
	uint3 groupThreadID : SV_GroupThreadID,
	uint3 dispatchID : SV_DispatchThreadID)
{
	Output[dispatchID.x].Number = Input[dispatchID.x].Number;

	Output[dispatchID.x].Id.x = groupID.x;
	Output[dispatchID.x].Id.y = groupThreadID.x;
	Output[dispatchID.x].Id.z = dispatchID.x;

	Output[dispatchID.x].Data = Input[dispatchID.x].Data;

}

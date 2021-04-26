//Byte.hlsl

ByteAddressBuffer Input; //SRV
RWByteAddressBuffer Output; //UAV

[numthreads(64,1,1)]
void CS(
	uint3 groupID : SV_GroupID,
	uint3 groupThreadID : SV_GroupThreadID,
	uint3 dispatchTreadID : SV_DispatchThreadID)
{
	uint fetchAddress = dispatchID * 6 * 4;

	uint number = asuint(Input.Load(fetchAddress + 0));
	float address = asfloat(Input.Load(fetchAddress + 4));
	float3 id = asfloat(Input.Load3(fetchAddress + 8));
	uint data = asuint(Input.Load(fetchAddress + 20));

	data *= 10;
	id.x = groupID;
	id.y = groupThreadID;
	id.z = dispatchID;

	address = fetchAddress;

	Output.Store(fetchAddress + 0, asuint(number));
	Output.Store(fetchAddress + 4, asuint(address));
	Output.Store3(fetchAddress + 8, asuint(id));
	Output.Store(fetchAddress + 20, asuint(data));

}
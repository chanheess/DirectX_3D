//FileName : TestInst.hlsl

cbuffer CB_ViewOrhto : register(b0)
{
    matrix View;
    matrix Projection;
};
cbuffer CB_World : register(b1)
{
    matrix World;
};
cbuffer CB_Color : register(b0)
{
	float4 Color = float4(1,1,1,1);
}

struct Vertex
{
    float4 Position : POSITION;
	float2 Uv : UV;
	float3 Index : INDEX;
	float4 TileUv : TILEUV; //xy 작은 uv ,zw 큰값
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
	float2 Uv : UV;
};

VertexOutput VS(Vertex input)
{
    VertexOutput output;
	//----------------------------------------------------------
	input.Position.x += input.Index.x;
	input.Position.y += input.Index.y;
	input.Position.z += input.Index.z;
	//----------------------------------------------------------

    //-------------- Rendering Bipeline -------------------------
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    //-------------- Rendering Bipeline -------------------------

	output.Uv.x = 
		input.Uv.x * (input.TileUv.z - input.TileUv.x) + input.TileUv.x;
	output.Uv.y = 
		input.Uv.y * (input.TileUv.w - input.TileUv.y) + input.TileUv.y;

    return output;
}

texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

float4 PS(VertexOutput input) : SV_TARGET
{
	float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
	float4 finalColor = Color;
	[flatten]
	//any -> 0이 아니면 
	if (any(diffuse))
	{
		finalColor = finalColor * diffuse;
	}
    return finalColor;
}
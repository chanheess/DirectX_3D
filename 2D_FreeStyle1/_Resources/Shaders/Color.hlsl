//FileName : Color.hlsl
//셰이더 파일을 작성하실때 주의점
//1.자동완성 기능이 없다.
//2.오탈자 검사가 안된다.
//3.hlsl 언어

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
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
	float2 Uv : UV;
};

VertexOutput VS(Vertex input)
{
    VertexOutput output;
    //-------------- Rendering Bipeline -------------------------
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    //-------------- Rendering Bipeline -------------------------

	output.Uv = input.Uv;
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
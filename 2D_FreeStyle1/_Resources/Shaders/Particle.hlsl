//Particle.hlsl

cbuffer ViewOrtho : register(b0)
{
    matrix View;
    matrix Projection;
}
cbuffer World : register(b1)
{
    matrix World;
}

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float2 Coord : COORD0;
    float4 Color : COLOR0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
    float4 Color : COLOR0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position.x = input.Position.x + input.Coord.x;
    output.Position.y = input.Position.y + input.Coord.y;
    output.Position.z = input.Position.z;
    output.Position.w = 1.0f;

    output.Position = mul(output.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;
    output.Color = input.Color;

    return output;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 TextureColor = DiffuseMap.Sample(Sampler, input.Uv);

    return input.Color * TextureColor;
};
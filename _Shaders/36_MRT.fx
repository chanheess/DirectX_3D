#include "00_Global.fx"
#include "00_Light.fx"

cbuffer CB_Render2D
{
    matrix View2D;
    matrix Projection2D;    //Ortho
};

struct VertexOutput
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
};

VertexOutput VS(VertexTexture input)
{
    VertexOutput output;

    output.Position = WorldPosition(input.Position);
    output.Position = mul(output.Position, View2D);
    output.Position = mul(output.Position, Projection2D);
    output.Uv = input.Uv;

    return output;
}

struct PixelOutput
{
    float4 Color0 : SV_Target0; //º»È­¸é
    float4 Color1 : SV_Target1; //Inverce
    float4 Color2 : SV_Target2; //Mono
    float4 Color3 : SV_Target3; //gamma
    float4 Color4 : SV_Target4; //bit planner slicing
    float4 Color5 : SV_Target5; //rgb
};

float gamma;
int bps;
PixelOutput PS(VertexOutput input)
{
    PixelOutput output;
    
    float3 color = DiffuseMap.Sample(LinearSampler, input.Uv).rgb;
    output.Color0 = float4(color, 1.0f);
    
    output.Color1 = float4(1.0f - color, 1.0f);
    
    float grayscale = (color.r + color.g + color.b) / 3.0f;
    output.Color2 =  float4(grayscale, grayscale, grayscale, 1.0f);
    
    float3 color2 = color;
    color2.r = pow(color.r, 1.0f / gamma);
    color2.g = pow(color.g, 1.0f / gamma);
    color2.b = pow(color.b, 1.0f / gamma);
    output.Color3 = float4(color2.r, color2.g, color2.b, 1.0f);
       
    uint r = uint(color.r * 255);
    uint g = uint(color.g * 255);
    uint b = uint(color.b * 255);
    r >>= (8 - bps);
    g >>= (8 - bps);
    b >>= (8 - bps);
    r <<= (8 - bps);
    g <<= (8 - bps);
    b <<= (8 - bps);
    output.Color4 = float4((r / 255.0f), (g / 255.0f), (b / 255.0f), 1.0f);
    
    output.Color5 = float4(color.r * 100, color.gb, 1.0f);
    
    return output;
}

DepthStencilState Depth
{
    DepthEnable = false;
};

technique11 T0
{
    P_DSS_VP(P0, Depth, VS, PS)
}
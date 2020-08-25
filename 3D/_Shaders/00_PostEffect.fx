cbuffer CB_PostEffect
{
    float2 PixelSize;
};

struct PostEffectValueDesc
{
    float Saturation;
    float Sharpness;
    float2 Padding;

    float2 VignetteScale;
    float VignettePower;

    float InteraceStrength;
    int InteraceValue;
    float3 Padding2;

    float3 Distortion;
    float LensPower;
    
    float Laplacian;
    float3 Padding3;
};

cbuffer CB_PostEffectValue
{
    PostEffectValueDesc PostEffectValue;
};

#define MAX_POSTEFFECT_SRVS 8
Texture2D Maps[MAX_POSTEFFECT_SRVS];

#define BlurCount 13
float BlurWeights[BlurCount] =
{
    0.002216, 0.008764, 0.026995, 0.064759, 0.120985, 0.176033,
    0.199471,
    0.176033, 0.120985, 0.064759, 0.026995, 0.008764, 0.002216
};

struct VertexOutput_PostEffect
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
};

VertexOutput_PostEffect VS_PostEffect(float4 Position : Position)
{
    VertexOutput_PostEffect output;
    output.Position = Position;
    output.Uv.x = Position.x * 0.5f + 0.5f;
    output.Uv.y = -Position.y * 0.5f + 0.5f;
    
    return output;
}
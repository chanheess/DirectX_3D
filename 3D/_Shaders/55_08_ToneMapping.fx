#include "00_Global.fx"
#include "00_Deffered.fx"
#include "00_PostEffect.fx"

float Lumiance = 0.08f;
float MiddleGray = 0.18f;
float WhiteCutOff = 0.8f;
float4 PS(VertexOutput_PostEffect input) : SV_Target
{
    float4 color = 0;
    color = Maps[0].Sample(LinearSampler, input.Uv);
    color *= MiddleGray / (Lumiance + 1e-6f);
    color *= (color / (WhiteCutOff * WhiteCutOff)) + 1.0f;
    color /= (color + 1.0f);
    
    return float4(color.rgb, 1.0f);
}


technique11 T0
{
    P_VP(P0, VS_PostEffect, PS)
}

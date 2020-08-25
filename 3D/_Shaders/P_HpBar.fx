#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS_HpBar(MeshOutput input) : SV_Target
{
    float4 color = float4(1, 0, 0, 1);
    
    return color;
}

float4 PS_HpBackground(MeshOutput input) : SV_Target
{
    float4 color = float4(0.6f, 0.6f, 0.6f, 1);
    
    return color;
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS_HpBar)
    P_VP(P1, VS_Mesh, PS_HpBackground)
}
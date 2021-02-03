#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float ratioHp;

cbuffer CB_Render2D
{
    matrix View2D;
    matrix Projection2D; //Ortho
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv;
};

VertexOutput VS_HpBar2D(VertexTexture input)
{
    VertexOutput output;

    output.Position = WorldPosition(input.Position);
    output.Position = mul(output.Position, View2D);
    output.Position = mul(output.Position, Projection2D);
    output.Uv = input.Uv;

    return output;
}

float4 PS_HpBar2D(VertexOutput input) : SV_Target
{
    float4 color;
    
    [flatten]
    if (input.Uv.x <= ratioHp)
    {
        color = float4(0, 0.5f, 0, 1);
        
        [flatten]
        if (ratioHp <= 0.25f)
        {
            color = float4(1, 0, 0, 1);
        }
    }
    
    [flatten]
    if (input.Uv.x > ratioHp)
    {
        color = float4(0.6f, 0.6f, 0.6f, 1);
    }
    
    return color;
}

float4 PS_HpBar(MeshOutput input) : SV_Target
{
    float4 color;
    
    [flatten]
    if (input.Uv.x <= ratioHp)
    {
        color = float4(0, 0.5f, 0, 1);
        
        [flatten]
        if (ratioHp <= 0.25f)
        {
            color = float4(1, 0, 0, 1);
        }
    }
    
    [flatten]
    if (input.Uv.x > ratioHp)
    {
        color = float4(0.6f, 0.6f, 0.6f, 1);
    }
    
    return color;
}

DepthStencilState Depth
{
    DepthEnable = false;
};

technique11 T0
{
    P_DSS_VP(P0, Depth, VS_Mesh, PS_HpBar)
    P_DSS_VP(P1, Depth, VS_HpBar2D, PS_HpBar2D)
}
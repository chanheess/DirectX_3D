#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Terrain.fx"

float4 PS(VertexTerrain input) : SV_Target
{
    float4 base = BaseMap.Sample(LinearSampler, input.Uv);
    float4 layer = LayerMap.Sample(LinearSampler, input.Uv);
    float4 color = lerp(base, layer, input.Uv.x);
    
    //float4 color = GetLayerColor(input.Uv);
    
    float NdotL = dot(-GlobalLight.Direction, normalize(input.Normal));
    float4 brush = GetBrushColor(input.wPosition);
    
    return (color * NdotL) + brush;
    //return lineColor;
}

float4 PS_GridLine(VertexTerrain input) : SV_Target
{
    float4 base = BaseMap.Sample(LinearSampler, input.Uv);
    float4 layer = LayerMap.Sample(LinearSampler, input.Uv);
    float4 color = lerp(base, layer, input.Uv.x);
    
    //float4 color = GetLayerColor(input.Uv);
    
    float NdotL = dot(-GlobalLight.Direction, normalize(input.Normal));
    
    float4 brushColor = GetBrushColor(input.wPosition);
    float4 lineColor = GetLineColor(input.wPosition);
    
    return (color * NdotL) + brushColor + lineColor;
    //return lineColor;
}


//int TextureInt;
//float4 PS_Spatting(VertexTerrain input) : SV_Target
//{
//    float4 base = BaseMap.Sample(LinearSampler, input.Uv);
//    float4 layer = LayerMap.Sample(LinearSampler, input.Uv);
//    float alpha = AlphaMap.Sample(LinearSampler, input.Uv).r;
    
//    float4 lerping = base;
//    //switch (TextureInt)
//    //{
//    //    case 0:
//    //        break;
//    //    case 1:
//    //        lerping = lerp(base, layer, input.Color.r);
//    //        break;
//    //    case 2:
//    //        break;
//    //    case 3:
//    //        break;
//    //}
//    ////float4 lerped = lerp(lerping, layer, (1 - alpha) * 0.5f);
    
//    ////float4 color = lerp(base, layer, input.Uv.x); //원래꺼 Uv.x 점점커진거
//    float4 colorlayer = GetLayerColor(input.Uv);
    
//    float NdotL = dot(-GlobalLight.Direction, normalize(input.Normal));
//    float4 brushColor = GetBrushColor(input.wPosition);
    
//    return (lerping * NdotL) + brushColor;
//}

technique11 T0
{
    P_VP(P0, VS_Terrain, PS)
    P_VP(P1, VS_Terrain, PS_GridLine)
    //P_VP(P2, VS_Terrain, PS_Spatting)
}
#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"
#include "00_Terrain.fx"

DepthOutput VS_Depth_TerrainLod(VertexInput_TerrainLod input)
{
    DepthOutput output;
    
    VS_DEPTH_GENERATE

    return output;
}

technique11 T0
{

    P_VTP(P0, VS_TerrainLod, HS_TerrainLod, DS_TerrainLod, PS_TerrainLod)
    P_RS_VTP(P1, FillMode_WireFrame, VS_TerrainLod, HS_TerrainLod, DS_TerrainLod, PS_TerrainLod)
    P_VTP(P2, VS_TerrainLod, HS_TerrainLod, DS_TerrainLod, PS_TerrainLod_Slope)

    //Depth
    P_RS_VP(P3, FrontCounterClockwise_True, VS_Depth_TerrainLod, PS_Depth)
    //Shadow
    P_VTP(P4, VS_TerrainLod, HS_TerrainLod, DS_TerrainLod, PS_TerrainLod_Shadow)
        
}
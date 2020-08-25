#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

float4 PS(MeshOutput input) : SV_Target
{
    Texture(Material.Diffuse, DiffuseMap, input.Uv);
    Texture(Material.Specular, SpecularMap, input.Uv);
    
    //Material.Diffuse = float4(0, 0, 0, 1);
    
    MaterialDesc result = MakeMaterial();
    ComputeLight(result, input.Normal, input.wPosition);
    
    return float4(MaterialToColor(result), 1.0f);
    //return Material.Emissive;

}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_VP(P1, VS_Model, PS)
    P_VP(P2, VS_Animation, PS)
}
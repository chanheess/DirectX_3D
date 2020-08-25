#include "00_Global.fx"
#include "00_Light.fx"

struct VertexInput
{
    float4 Position : Position;
    float2 Scale : Scale;
    uint Count : Count;
    
    //pointlist에서만 사용가능
    uint VertexID : SV_VertexID; //정점의 순번을 알수있음
};

struct VertexOutput
{
    float4 Position : Position; //GS로 가기때문에 SV_를 뻄
    float Scale : Scale;
    uint Count : Count;
    
    uint VertexID : VertexID;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = WorldPosition(input.Position);
    output.Scale = input.Scale;
    output.Count = input.Count;
       
    output.VertexID = input.VertexID;
    
    return output;
}

struct GeometryOutput
{
    float4 Position : SV_Position;  //PS로 가는것만 SV_가 붙음
    float2 Uv : Uv;
    uint Count : Count;
    
    uint VertexID : VertexID; //정점의 순번을 알수있음
};

[maxvertexcount(4)] //한정점당 네개씩
void GS(point VertexOutput input[1], inout TriangleStream<GeometryOutput> stream)
{
    float3 up = float3(0, 1, 0);
    //float3 forward = float3(0, 0, 1);
    float3 forward = input[0].Position.xyz - ViewPosition();
    float3 right = normalize(cross(up, forward));
    
    float2 size = input[0].Scale * 0.5f;
    
    float4 position[4];
    position[0] = float4(input[0].Position.xyz - size.x * right - size.y * up, 1);
    position[1] = float4(input[0].Position.xyz - size.x * right + size.y * up, 1);
    position[2] = float4(input[0].Position.xyz + size.x * right - size.y * up, 1);
    position[3] = float4(input[0].Position.xyz + size.x * right + size.y * up, 1);
    
    float2 uv[4] =
    {
        float2(0, 1), float2(0, 0), float2(1, 1), float2(1, 0)
    };
    
    GeometryOutput output;
    
    [unroll(4)]
    for(int i = 0; i < 4; i++)
    {
        output.Position = ViewProjection(position[i]);
        output.Uv = uv[i];
        output.VertexID = input[0].VertexID;
        output.Count = input[0].Count;
        
        stream.Append(output);
    }
    
}
    
Texture2DArray Maps;
float4 PS(GeometryOutput input) : SV_Target
{
    //float4 diffuse = Maps.Sample(LinearSampler, float3(input.Uv, input.VertexID % 6));
    float4 diffuse = Maps.Sample(LinearSampler, float3(input.Uv, input.Count));
    
    return diffuse;
    
    //return float4((float) input.VertexID / 100.0f, 0, 0, 1);
    
}

technique11 T0
{
    P_BS_VGP(P0, AlphaBlend_AlphaToCoverageEnable, VS, GS, PS)
}
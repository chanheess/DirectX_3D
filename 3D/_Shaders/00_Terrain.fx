Texture2D BaseMap;

Texture2D LayerMap1;
Texture2D LayerMap2;
Texture2D LayerMap3;

Texture2D LayerSlope1;
Texture2D LayerSlope2;

texture2D AlphaMap;
texture2D HeightMap;

struct VertexTerrain
{
    float4 Position : SV_Position;
    float3 wPosition : Position1;
    float2 Uv : Uv;
    float3 Normal : Normal;
};

VertexTerrain VS_Terrain(VertexTextureNormal input)
{
    VertexTerrain output;
    output.Position = mul(input.Position, World);
    output.wPosition = output.Position.xyz;
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;
    
    return output;
}

///////////////////////////////////////////////////////////////////////////////

float4 GetLayerColor(float2 uv)
{
    float4 base = BaseMap.Sample(LinearSampler, uv);
    
    return base;
}
float4 GetLayerColor1(float4 color, float2 uv)
{
    float4 layer = LayerMap1.Sample(LinearSampler, uv);
    float alpha = AlphaMap.Sample(LinearSampler, uv).r;
    
    return lerp(color, layer, alpha);
}
float4 GetLayerColor2(float4 color, float2 uv)
{
    float4 layer = LayerMap2.Sample(LinearSampler, uv);
    float alpha = AlphaMap.Sample(LinearSampler, uv).g;
    return lerp(color, layer, alpha);
}
float4 GetLayerColor3(float4 color, float2 uv)
{
    float4 layer = LayerMap3.Sample(LinearSampler, uv);
    float alpha = AlphaMap.Sample(LinearSampler, uv).b;
    return lerp(color, layer, alpha);
}

///////////////////////////////////////////////////////////////////////////////

struct BrushDesc
{
    float4 Color;
    float3 Location;
    uint Type;
    uint HeightType;
    uint SlopeDirection;
    uint Range;
    uint Range2;
};

cbuffer CB_Brush
{
    BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{
    [flatten]
    if(TerrainBrush.Type == 1)
    {
        [flatten]
        if((wPosition.x >= (TerrainBrush.Location.x - TerrainBrush.Range)) &&
            (wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range)) &&
            (wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range)) &&
            (wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range)) &&
            TerrainBrush.HeightType != 2)
        {
            return TerrainBrush.Color;
        }
        
        [flatten]
        if ((wPosition.x >= TerrainBrush.Location.x - TerrainBrush.Range) &&
            (wPosition.x <= TerrainBrush.Location.x) &&
            (wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range2)) &&
            (wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range2)) &&
            TerrainBrush.HeightType == 2 && TerrainBrush.SlopeDirection == 0) //left
        {
            return TerrainBrush.Color;
        }
        
        [flatten]
        if ((wPosition.x >= TerrainBrush.Location.x) &&
            (wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range)) &&
            (wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range2)) &&
            (wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range2)) &&
            TerrainBrush.HeightType == 2 && TerrainBrush.SlopeDirection == 1) //Right
        {
            return TerrainBrush.Color;
        }
        
        [flatten]
        if ((wPosition.x >= TerrainBrush.Location.x - TerrainBrush.Range2) &&
            (wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range2)) &&
            (wPosition.z >= (TerrainBrush.Location.z)) &&
            (wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range)) &&
            TerrainBrush.HeightType == 2 && TerrainBrush.SlopeDirection == 2) //Up
        {
            return TerrainBrush.Color;
        }
        
        [flatten]
        if ((wPosition.x >= TerrainBrush.Location.x - TerrainBrush.Range2) &&
            (wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range2)) &&
            (wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range)) &&
            (wPosition.z <= (TerrainBrush.Location.z)) &&
            TerrainBrush.HeightType == 2 && TerrainBrush.SlopeDirection == 3) //Down
        {
            return TerrainBrush.Color;
        }
        
        
    }
    
    [flatten]
    if(TerrainBrush.Type == 2)
    {
        float dx = wPosition.x - TerrainBrush.Location.x;
        float dz = wPosition.z - TerrainBrush.Location.z;
        float dist = sqrt(dx * dx + dz * dz);
        
        [flatten]
        if(dist <= (float)TerrainBrush.Range)
            return TerrainBrush.Color;
    }
    
    return float4(0, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////

struct LineDesc
{
    float4 Color;
    uint Visible;
    float Thickness;
    float Size;
};

cbuffer CB_TerrainLine
{
    LineDesc TerrainLine;
};

float4 GetBasicLineColor(float3 wPosition)
{
    [flatten]
    if(TerrainLine.Visible < 1)
        return float4(0, 0, 0, 0);
    
    float2 grid = wPosition.xz / TerrainLine.Size;
    grid = frac(grid);
    
    float thick = TerrainLine.Thickness / TerrainLine.Size;
    
    [flatten]
    if (grid.x < thick || grid.y < thick)
        return TerrainLine.Color;
    
    return float4(0, 0, 0, 0);
}

float4 GetLineColor(float3 wPosition)
{
    [flatten]
    if (TerrainLine.Visible < 1)
        return float4(0, 0, 0, 0);
    
    //float2 grid = wPosition.xz;
    float2 grid = wPosition.xz / TerrainLine.Size;
    float2 range = abs(frac(grid - 0.5f) - 0.5f);
    //return float4(range, 0, 1);
    
    float2 speed = fwidth(grid);
    //return float4(speed, 0, 1);
    
    float2 pixel = range / speed;
    //return float4(pixel, 0, 1);
    
    float thick = saturate(min(pixel.x, pixel.y) - TerrainLine.Thickness);
    //return float4(pixel, 0, 1);
    
    return lerp(TerrainLine.Color, float4(0, 0, 0, 0), thick);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// TerrainLod
////////////////////////////////////////////////////////////////////////////////////////////////

cbuffer CB_TerrainLod
{
    float MinDistance;  //LOD가 될 최소거리
    float MaxDistance;  //LOD가 될 최대거리
    float MinTessellation;  //최소개수 2
    float MaxTessellation;  //최대개수 64

    float TexelCellSpaceU;  //uv 한칸의 크기
    float TexelCellSpaceV;  //uv 한칸의 크기
    float WorldCellSpace;
    float TerrainHeightRatio;   //height 낮추는 비율
    
    float4 WorldFrustumPlanes[6];   //면들의 컬링용
};

struct VertexInput_TerrainLod
{
    float4 Position : Position;
    float2 Uv : Uv;
    float2 BoundsY : Bound;
    
};

struct VertexOutput_TerrainLod
{
    float4 Position : Position;
    float2 Uv : Uv;
    float2 BoundsY : Bound;
};

VertexOutput_TerrainLod VS_TerrainLod(VertexInput_TerrainLod input)
{
    VertexOutput_TerrainLod output;
    
    output.Position = input.Position;
    output.Uv = input.Uv;
    output.BoundsY = input.BoundsY;
    
    return output;
}

struct CHullOutput_TerrainLod
{
    float Edge[4] : SV_TessFactor;
    float Inside[2] : SV_InsideTessFactor;
};

float TessellationFactor(float3 position)
{
    position = float3(position.x, 0.0f, position.z);
    float3 view = float3(ViewPosition().x, 0.0f, ViewPosition().z);
    
    float d = distance(position, view);
    float s = saturate((d - MaxDistance) / (MinDistance - MaxDistance));
    
    return lerp(MinTessellation, MaxTessellation, s);
}

bool PlaneTest(float3 center, float3 extents, float4 plane)
{
    float3 n = abs(plane.xyz);
    float r = dot(extents, n);
    float s = dot(float4(center, 1), plane);
    
    return (s + r) < 0.0f;
}

bool InFrustumCube(float3 center, float3 extents)
{
    [unroll(6)]
    for (int i = 0; i < 6; i++)
    {
        [flatten]
        if(PlaneTest(center, extents, WorldFrustumPlanes[i]))
            return true;    //컬링된얘
    }
    
    return false;
}

CHullOutput_TerrainLod
    CHS_TerrainLod(
    InputPatch<VertexOutput_TerrainLod, 4> input)
{
    float minY = input[0].BoundsY.x;
    float maxY = input[0].BoundsY.y;
    
    float3 vMin = float3(input[2].Position.x, minY, input[2].Position.z);
    float3 vMax = float3(input[1].Position.x, minY, input[1].Position.z);
    
    float3 boxCenter = (vMin + vMax) * 0.5f;
    float3 boxExtents = (vMax - vMin) * 0.5f;
    
    
    CHullOutput_TerrainLod output;
    
    [flatten]
    if (InFrustumCube(boxCenter, boxExtents)) //컬링이 됐을때
    {
        output.Edge[0] = output.Edge[1] = output.Edge[2] = output.Edge[3] = -1;
        output.Inside[0] = output.Inside[1] = -1;

        return output;
    }

    float3 e0 = (input[0].Position + input[2].Position).xyz * 0.5f;
    float3 e1 = (input[0].Position + input[1].Position).xyz * 0.5f;
    float3 e2 = (input[1].Position + input[3].Position).xyz * 0.5f;
    float3 e3 = (input[2].Position + input[3].Position).xyz * 0.5f;
    
    output.Edge[0] = TessellationFactor(e0);
    output.Edge[1] = TessellationFactor(e1);
    output.Edge[2] = TessellationFactor(e2);
    output.Edge[3] = TessellationFactor(e3);
    
    float3 c = (input[0].Position + input[1].Position + input[2].Position + input[3].Position).xyz * 0.25f;
    output.Inside[0] = TessellationFactor(c);
    output.Inside[1] = TessellationFactor(c);
    
    return output;
}

struct HullOutput_TerrainLod
{
    float4 Position : Position;
    float2 Uv : Uv;
};

[domain("quad")]
//[partitioning("integer")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS_TerrainLod")]
HullOutput_TerrainLod HS_TerrainLod(InputPatch<VertexOutput_TerrainLod, 4> input, uint id :SV_OutputControlPointID)
{
    HullOutput_TerrainLod output;
    
    output.Position = input[id].Position;
    output.Uv = input[id].Uv;
    
    return output;
}

struct DomainOutput_TerrainLod
{
    float4 Position : SV_Position;
    float3 wPosition : Position1;
    float2 Uv : Uv;
};

[domain("quad")]
DomainOutput_TerrainLod DS_TerrainLod(CHullOutput_TerrainLod input, OutputPatch<HullOutput_TerrainLod, 4> patch, float2 uv : SV_DomainLocation)
{
    DomainOutput_TerrainLod output;

    float3 p0 = lerp(patch[0].Position, patch[1].Position, uv.x).xyz;
    float3 p1 = lerp(patch[2].Position, patch[3].Position, uv.x).xyz;
    float3 position = lerp(p0, p1, uv.y);
    
    float2 uv0 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
    float2 uv1 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
    output.Uv = lerp(uv0, uv1, uv.y);
    
    position.y = HeightMap.SampleLevel(LinearSampler, output.Uv, 0).r * 255 / TerrainHeightRatio;
    
    output.Position = WorldPosition(float4(position, 1));
    output.wPosition = output.Position.xyz;
    
    output.Position = ViewProjection(output.Position);
    
    return output;
}

float4 PS_TerrainLod(DomainOutput_TerrainLod input) : SV_Target
{
    float2 left = input.Uv + float2(-TexelCellSpaceU, 0.0f);
    float2 right = input.Uv + float2(+TexelCellSpaceU, 0.0f);
    float2 top = input.Uv + float2(0.0f, -TexelCellSpaceV);
    float2 bottom = input.Uv + float2(0.0f, TexelCellSpaceV);
    
    float leftY = HeightMap.SampleLevel(LinearSampler, left, 0).r * 255 / TerrainHeightRatio;
    float rightY = HeightMap.SampleLevel(LinearSampler, right, 0).r * 255 / TerrainHeightRatio;
    float topY = HeightMap.SampleLevel(LinearSampler, top, 0).r * 255 / TerrainHeightRatio;
    float bottomY = HeightMap.SampleLevel(LinearSampler, bottom, 0).r * 255 / TerrainHeightRatio;
    
    float3 tangent = normalize(float3(WorldCellSpace * 2.0f, rightY - leftY, 0.0f));
    float3 biTangent = normalize(float3(0.0f, bottomY - topY, WorldCellSpace * -2.0f));
    float3 normal = normalize(cross(tangent, biTangent));
   
    float4 color = GetLayerColor(input.Uv);
    float4 currColor = color;
    
    NormalMapping(input.Uv, normal, tangent);
    
    float4 brush = GetBrushColor(input.wPosition);
    

    float4 colors1 = GetLayerColor1(currColor, input.Uv);
    currColor = colors1;
    float4 colors2 = GetLayerColor2(currColor, input.Uv);
    currColor = colors2;
    float4 colors3 = GetLayerColor3(currColor, input.Uv);
    currColor = colors3;
    
    return currColor + brush;
}

float4 PS_TerrainLod_Slope(DomainOutput_TerrainLod input) : SV_Target
{
    float2 left = input.Uv + float2(-TexelCellSpaceU, 0.0f);
    float2 right = input.Uv + float2(+TexelCellSpaceU, 0.0f);
    float2 top = input.Uv + float2(0.0f, -TexelCellSpaceV);
    float2 bottom = input.Uv + float2(0.0f, TexelCellSpaceV);
    
    float leftY = HeightMap.SampleLevel(LinearSampler, left, 0).r * 255 / TerrainHeightRatio;
    float rightY = HeightMap.SampleLevel(LinearSampler, right, 0).r * 255 / TerrainHeightRatio;
    float topY = HeightMap.SampleLevel(LinearSampler, top, 0).r * 255 / TerrainHeightRatio;
    float bottomY = HeightMap.SampleLevel(LinearSampler, bottom, 0).r * 255 / TerrainHeightRatio;
    
    float3 tangent = normalize(float3(WorldCellSpace * 2.0f, rightY - leftY, 0.0f));
    float3 biTangent = normalize(float3(0.0f, bottomY - topY, WorldCellSpace * -2.0f));
    float3 normal = normalize(cross(tangent, biTangent));
   
    float4 color = GetLayerColor(input.Uv);
    float4 currColor = color;
    
    NormalMapping(input.Uv, normal, tangent);
    
    float4 brush = GetBrushColor(input.wPosition);
    

    float4 colors1 = GetLayerColor1(currColor, input.Uv);
    currColor = colors1;
    float4 colors2 = GetLayerColor2(currColor, input.Uv);
    currColor = colors2;
    float4 colors3 = GetLayerColor3(currColor, input.Uv);
    currColor = colors3;
    
    float slope;
    float blendAmount;
    slope = 1.0f - normal.y;
    
    [flatten]
    if (slope < 0.2)
    {
        blendAmount = slope / 0.2f;
        float4 layer1 = LayerSlope1.Sample(LinearSampler, input.Uv);
        currColor = lerp(currColor, layer1, blendAmount);
    }
    
    [flatten]
    if ((slope < 0.7) && (slope >= 0.2f))
    {
        blendAmount = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
        float4 layer1 = LayerSlope1.Sample(LinearSampler, input.Uv);
        float4 layer2 = LayerSlope2.Sample(LinearSampler, input.Uv);
        currColor = lerp(layer1, layer2, blendAmount);
    }
    
    [flatten]
    if (slope >= 0.7)
    {
        float4 layer3 = LayerSlope2.Sample(LinearSampler, input.Uv);
        currColor = layer3;
    }
    
    return currColor + brush;
}

float4 PS_TerrainLod_Shadow(DomainOutput_TerrainLod input) : SV_Target
{
    float2 left = input.Uv + float2(-TexelCellSpaceU, 0.0f);
    float2 right = input.Uv + float2(+TexelCellSpaceU, 0.0f);
    float2 top = input.Uv + float2(0.0f, -TexelCellSpaceV);
    float2 bottom = input.Uv + float2(0.0f, TexelCellSpaceV);
    
    float leftY = HeightMap.SampleLevel(LinearSampler, left, 0).r * 255 / TerrainHeightRatio;
    float rightY = HeightMap.SampleLevel(LinearSampler, right, 0).r * 255 / TerrainHeightRatio;
    float topY = HeightMap.SampleLevel(LinearSampler, top, 0).r * 255 / TerrainHeightRatio;
    float bottomY = HeightMap.SampleLevel(LinearSampler, bottom, 0).r * 255 / TerrainHeightRatio;
    
    float3 tangent = normalize(float3(WorldCellSpace * 2.0f, rightY - leftY, 0.0f));
    float3 biTangent = normalize(float3(0.0f, bottomY - topY, WorldCellSpace * -2.0f));
    float3 normal = normalize(cross(tangent, biTangent));
   
    float4 color = GetLayerColor(input.Uv);
    float4 currColor = color;
    
    NormalMapping(input.Uv, normal, tangent);
    
    float4 brush = GetBrushColor(input.wPosition);
    

    float4 colors1 = GetLayerColor1(currColor, input.Uv);
    currColor = colors1;
    float4 colors2 = GetLayerColor2(currColor, input.Uv);
    currColor = colors2;
    float4 colors3 = GetLayerColor3(currColor, input.Uv);
    currColor = colors3;
    
    float NdotL = dot(-GlobalLight.Direction, normalize(normal));
   
    float4 result = currColor * NdotL + brush;
    
    
    return result;
}

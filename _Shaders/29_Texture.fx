Texture2DArray<float4> Input;
RWTexture2DArray<float4> Output; //RW읽고쓰기

float4 Setting;

[numthreads(32,32,1)]
void CS(uint3 id : SV_DispatchThreadID)
{
    float4 color = Input.Load(int4(id, 0));

    [branch]
    switch (Setting.x)
    {
        case 0:
            Output[id] = float4(color.rgb, 1);
            break;
        case 1:
            Output[id] = float4(1.0f - color.rgb, 1); //invert
            break;
        case 2:
            Output[id] = (color.r + color.g + color.b) / 3.0f;
            break;
        case 3:
            uint r = uint(color.r * 255);
            uint g = uint(color.g * 255);
            uint b = uint(color.b * 255);

            r >>= (8 - (int)Setting.y);
            g >>= (8 - (int)Setting.y);
            b >>= (8 - (int)Setting.y);
    
            r <<= (8 - (int)Setting.y);
            g <<= (8 - (int)Setting.y);
            b <<= (8 - (int)Setting.y);
        
            Output[id] = float4((r / 255.0f), (g / 255.0f), (b / 255.0f), 1.0f);
            break;
        case 4:
            color.r = pow(color.r, 1.0f / Setting.y);
            color.g = pow(color.g, 1.0f / Setting.y);
            color.b = pow(color.b, 1.0f / Setting.y);
        
            Output[id] = float4(color.rgb, 1.0f);
            break;
    }
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(NULL);
        SetPixelShader(NULL);

        SetComputeShader(CompileShader(cs_5_0, CS()));
    }

}


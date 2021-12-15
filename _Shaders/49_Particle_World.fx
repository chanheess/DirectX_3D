#include "00_Global.fx"

struct ParticleDesc
{
    float4 MinColor;
    float4 MaxColor;
    
    float3 Gravity;
    float EndVelocity;
    
    float2 StartSize;
    float2 EndSize;
    
    float2 RotateSpeed;
    float Duration;
    float DurationRandomness;
    
    float CurrentTime;
};

cbuffer CB_Particle
{
    ParticleDesc Particle;
};

struct InputDesc
{
    float3 Position;
    float3 Velocity;
    float4 Random : Random0;
    float Time : Time0;
    
};
StructuredBuffer<InputDesc> Input;

struct OutputDesc
{
    matrix World;
};
RWStructuredBuffer<OutputDesc> Output;


float3 ComputePosition(float3 position, float3 velocity, float age, float normalizedAge)
{
    float startVelocity = length(velocity);
    float endVelocity = startVelocity * Particle.EndVelocity;
    
    float velocityIntegral = startVelocity * normalizedAge + (endVelocity - startVelocity) * normalizedAge / 2;

    position += normalize(velocity) * velocityIntegral * Particle.Duration;
    position += Particle.Gravity * age * normalizedAge;
    
    return position;
}

float ComputeSize(float randomValue, float normalizedAge)
{
    float startSize = lerp(Particle.StartSize.x, Particle.StartSize.y, randomValue);
    float endSize = lerp(Particle.EndSize.x, Particle.EndSize.y, randomValue);
    
    return lerp(startSize, endSize, normalizedAge);
}

float2x2 ComputeRotation(float randomValue, float age)
{
    float rotationSpeed = lerp(Particle.RotateSpeed.x, Particle.RotateSpeed.y, randomValue);
    float rotation = rotationSpeed * age;
    
    float c = cos(rotation);
    float s = sin(rotation);
    
    return float2x2(c, -s, s, c);
}

[numthreads(1024, 1, 1)]
void CS(uint3 GroupID : SV_GroupID, uint GroupIndex : SV_GroupIndex)
{
    uint index = GroupID.x * 1024 + GroupIndex;
    
    float age = Particle.CurrentTime - Input[index].Time;
    age *= Input[index].Random.x * Particle.DurationRandomness + 1;
    
    float normalizedAge = saturate(age / Particle.Duration);
    
    float3 position = float3(0, 0, 0);
    position = ComputePosition(Input[index].Position, Input[index].Velocity, age, normalizedAge);
    
    float size = ComputeSize(Input[index].Random.y, normalizedAge);
    float2x2 rotation = ComputeRotation(Input[index].Random.w, age);
    
    matrix s = matrix
    (
        size, 0, 0, 0,
        0, size, 0, 0,
        0, 0, size, 0,
        0, 0, 0, 1
    );
    
    matrix r = matrix
    (
        rotation._11, rotation._12, 0, 0,
        rotation._21, rotation._22, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    
    matrix t = matrix
    (
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        position.x, position.y, position.z, 1
    );
    
    Output[index].World = mul(mul(s, r), t);
}
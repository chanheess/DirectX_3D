#include "stdafx.h"
#include "Particle.h"

#include "Renderer/Buffers/VBuffer.h"
#include "Renderer/Buffers/IBuffer.h"
#include "Renderer/Texture.h"

Particle::Particle(float spawnTime, UINT totalNum, float minSpeed, float maxSpeed, 
	float minAccel, float maxAccel, float minLiveTime, float maxLiveTime,
	Vector2 minPos, Vector2 maxPos, Vector2 minDir, Vector2 maxDir, vector<D3DXCOLOR> color)
	: Component(L"ParticleSystem"),
	spawnTime(spawnTime),
	totalNum(totalNum),
	minSpeed(minSpeed),
	maxSpeed(maxSpeed),
	minAccel(minAccel),
	maxAccel(maxAccel),
	minLiveTime(minLiveTime),
	maxLiveTime(maxLiveTime),
	minPos(minPos),
	maxPos(maxPos),
	minDir(minDir),
	maxDir(maxDir),
	color(color)
{
	currentNum = 0;
	spawn = 0;

	VertexTexture* vertices = new VertexTexture[4];
	vertices[0] = VertexTexture(-0.5f, -0.5f, 0, 1);
	vertices[1] = VertexTexture(-0.5f,  0.5f, 0, 0);
	vertices[2] = VertexTexture( 0.5f,  0.5f, 1, 0);
	vertices[3] = VertexTexture( 0.5f, -0.5f, 1, 1);

	vBuffer = new VBuffer(vertices, sizeof(VertexTexture), 4);

	UINT* indices = new UINT[6]{ 0,1,2,0,2,3 };
	iBuffer = new IBuffer(indices, 6);

	instance = new Instancing[totalNum];
	ZeroMemory(instance, sizeof(Instancing) * totalNum);

	instacing = new VBuffer(instance, sizeof(Instancing), totalNum);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);


	shader = new Shader(L"Particle.hlsl");
	shader->CreateInputLayout(VTIPC::desc, VTIPC::count);

	texture = new Texture(L"particle_0.png");
}

Particle::~Particle()
{
	SAFE_DELETE(vBuffer);
	SAFE_DELETE(iBuffer);
	SAFE_DELETE(instacing);
	SAFE_DELETE_ARRAY(instance);
}

void Particle::Update()
{
	KillParticle();

	UpdateParticle();

	spawn += 0.0016f;

	EmitParticle();

	instacing->SetData(instance);
}

void Particle::Render()
{
	shader->Render();
	vBuffer->Render(instacing);
	iBuffer->Render();
	texture->Render();
	D2D::GetDC()->IASetPrimitiveTopology(TRIANGLELIST);
	D2D::GetDC()->DrawIndexedInstanced(6, currentNum, 0, 0, 0);
}

void Particle::EmitParticle()
{
	bool bSpawn = false;

	if (spawn >= spawnTime)
	{
		spawn -= spawnTime;
		bSpawn = true;
	}

	if (bSpawn && currentNum < totalNum)
	{
		float x = Math::Random(minPos.x, maxPos.x);
		float y = Math::Random(minPos.y, maxPos.y);

		float dirX = Math::Random(minDir.x, maxDir.x);
		float dirY = Math::Random(minDir.y, maxDir.y);

		float speed = Math::Random(minSpeed, maxSpeed);
		float accel = Math::Random(minAccel, maxAccel);

		float liveTime = Math::Random(minLiveTime, maxLiveTime);

		instance[currentNum].LiveTime = Vector2(liveTime, 0);
		instance[currentNum].Color = D3DXCOLOR(0, 0, 0, 1);
		instance[currentNum].SpeedAndAccel = Vector2(speed, accel);
		instance[currentNum].Direction = Vector2(dirX, dirY);
		instance[currentNum].Coord = Vector2(x, y);
		instance[currentNum].bAlive = true;
		currentNum++;
	}

	if (spawn >= spawnTime) EmitParticle();
}

void Particle::UpdateParticle()
{
	for (int i = 0; i < currentNum; i++)
	{
		instance[i].LiveTime.y += 0.0016;
		if (instance[i].LiveTime.y >= instance[i].LiveTime.x)
		{
			instance[i].bAlive = false;
			instance[i].Color = color[color.size() - 1];
		}

		if (instance[i].bAlive == false) continue;

		instance[i].Coord += instance[i].Direction
			* instance[i].SpeedAndAccel.x * 0.0016;
		instance[i].SpeedAndAccel.x += instance[i].SpeedAndAccel.y
			* 0.0016;

		int lastIndex = color.size() - 1;

		float interval = 1.0f / lastIndex;

		float position = instance[i].LiveTime.y / instance[i].LiveTime.x / interval;
		
		int startIndex = (int)position;

		float s = position - startIndex;

		if (startIndex == color.size() - 1)
			int a = 10;
		D3DXColorLerp(&instance[i].Color,
			&color[startIndex],
			&color[startIndex + 1], s);
	}
}

void Particle::KillParticle()
{
	for (int i = 0; i < currentNum; i++)
	{
		if (instance[i].bAlive == false)
		{
			currentNum--;
			for (int j = i; j < currentNum - 1; j++)
			{
				instance[j] = instance[j + 1];
			}
		}
	}


}

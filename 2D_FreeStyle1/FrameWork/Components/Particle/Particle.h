#pragma once
#include "Components/Component.h"
//입자 
class Particle : public Component
{
public:
	Particle(
		float		spawnTime,
		UINT		totalNum,
		float		minSpeed,
		float		maxSpeed,
		float		minAccel,
		float		maxAccel,
		float		minLiveTime,
		float		maxLiveTime,
		Vector2		minPos,
		Vector2		maxPos,
		Vector2		minDir,
		Vector2		maxDir,
		vector<D3DXCOLOR> color
	);
	~Particle();

	void Update();
	void Render();

private:
	void EmitParticle(); //입자 1개를 생성하겠다.
	void UpdateParticle(); //입자들을 이동하겠다.
	void KillParticle();	//화면에 출력시키지 않겠다.
private:

	UINT totalNum; //총입자수 
	UINT currentNum;

	Vector2 minPos;
	Vector2 maxPos;

	Vector2 minDir;
	Vector2 maxDir;

	float minSpeed;
	float maxSpeed;

	float minAccel;
	float maxAccel;

	float minLiveTime;
	float maxLiveTime;

	vector<D3DXCOLOR> color;

	/*D3DXCOLOR minColor;
	D3DXCOLOR maxColor;*/

	float spawnTime;
	float spawn;
private:
	struct Instancing
	{
		Vector2 Coord;
		D3DXCOLOR Color;

		bool bAlive;
		Vector2 LiveTime;
		Vector2 Direction;
		Vector2 SpeedAndAccel;
	}* instance;
private:
	VertexTexture * vertices;
	UINT* indices;
	
	class Shader* shader;
	class VBuffer* vBuffer;
	class VBuffer* instacing;
	class IBuffer* iBuffer;
	class Texture* texture;
};


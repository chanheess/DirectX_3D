#pragma once
#include "Systems/IExecute.h"
#define PLAYERCOUNT 1
#define WEAPONCOUNT 2
#define MONSTERCOUNT 6
#define RANGEMONSTERCOUNT 2
#define PARTICLECOUNT 4
#define ARROWCOUNT 1

class Battle : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override; 
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:

	//void Mesh();
	void Player();
	void Monster();
	void NoAttachWeapons();

	void Pass(UINT mesh, UINT model, UINT anim);

	void PlayerAttackControl(UINT attackPlayer);
	void MonsterAttackControl(UINT attackMonster, Vector3 playerPos);
	void RangeAttacked(Vector3 pos);
	void PlayerArrowUpdates();
	void MonsterArrowUpdates(UINT input);

	void AddSpotLights();

private:
	ModelRender* tree;

	Shader* shader;
	Shadow* shadow;

private:	//range skill
	ParticleSystem* explosion[PARTICLECOUNT];
	float rangeSkillDelay = 0;
	int rangeSkillCount = 0;
	bool rangeSkillOn = false;
	Vector3 rangeCameraPos{ 0,0,0 };
	Vector3 rangeSkillPos = { 0,0,0 };

private:
	//Player & monsters
	enum PlayerAttackState
	{
		Idle = 0,
		SwordAttack = 1,
		PunchAttack = 2,
		RangeAttack = 3,
		DistanceAttack = 4
	} playerAttackState;

	vector<PlayerAttackState> AttackState;
	vector<PlayerAttackState> MonsterAttackState;
	vector<UINT> noAttack;	//플레이어 공격 애니메이션 컨트롤
	vector<UINT> monsterNoAttack;

	class HpBar* playerHp;
	class HpBar* monsterHp[MONSTERCOUNT + RANGEMONSTERCOUNT];

	bool playerDeath = false;	//플레이어 죽었나

	UINT dontAttack = 0;

private:
	//활
	Vector3 arrowStartPos;
	Vector3 arrowStartRot;
	Vector3 arrowStartScale;
	float dontShootTime = 0;	//활쏘기 쿨타임
	bool shootTime = false;	//활쏘기 쿨타임용
	bool shootMotion = false;	//활쏘기 모션
	vector<float> playerTime;
	vector<float> monsterTime;	//나중에 리스폰을 만들자!
	vector<float> rangeMonsterTime;	//활 전용
	float timing = 0.0f;
	
private:
	ColliderObjectDesc** playerColliders;
	ColliderObjectDesc** playerArrowColliders;
	ColliderObjectDesc** weaponColliders;
	ColliderObjectDesc** dropWeaponColliders;

	ColliderObjectDesc** monsterColliders;
	ColliderObjectDesc** monsterAttackColliders;
	ColliderObjectDesc** monsterArrowColliders;
	ColliderObjectDesc** rangeMonsterColliders;

	Model* weapon[WEAPONCOUNT];
	Model* monsterRangeWeapon[RANGEMONSTERCOUNT];

	//맞았을때 한번만 맞게
	bool monsterToPlayer[MONSTERCOUNT + RANGEMONSTERCOUNT]{ false };	//몬스터가 플레이어 공격
	bool playerToMonster[MONSTERCOUNT + RANGEMONSTERCOUNT]{ false };	//플레이어가 몬스터 공격

	ModelRender* dropWeapons[WEAPONCOUNT];
	ModelRender* playerArrows[ARROWCOUNT];
	ModelRender* monsterArrows[RANGEMONSTERCOUNT];
	bool getWeaponed[2] = { false };

	ModelAnimator* player = NULL;
	ModelAnimator* monster = NULL;
	ModelAnimator* rangeMonster = NULL;

	ModelControler* playerControler[PLAYERCOUNT];
	ModelControler* monsterControler[MONSTERCOUNT];
	ModelControler* rangeMonsterControler[RANGEMONSTERCOUNT];

	Vector3 mousePos{0,0,0};	//mousePos

private:
	Sky* skyDome;
	Water* water;

	TerrainLod* terrain;
	Shader* terrainShader;

	//Material* floor;
	//MeshRender* grid;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;

	VertexTextureNormalTangent* vertices;

	bool isRender = false;
	bool battleStyle = false;
};
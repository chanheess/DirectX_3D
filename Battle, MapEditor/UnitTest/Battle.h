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
	vector<UINT> noAttack;	//�÷��̾� ���� �ִϸ��̼� ��Ʈ��
	vector<UINT> monsterNoAttack;

	class HpBar* playerHp;
	class HpBar* monsterHp[MONSTERCOUNT + RANGEMONSTERCOUNT];

	bool playerDeath = false;	//�÷��̾� �׾���

	UINT dontAttack = 0;

private:
	//Ȱ
	Vector3 arrowStartPos;
	Vector3 arrowStartRot;
	Vector3 arrowStartScale;
	float dontShootTime = 0;	//Ȱ��� ��Ÿ��
	bool shootTime = false;	//Ȱ��� ��Ÿ�ӿ�
	bool shootMotion = false;	//Ȱ��� ���
	vector<float> playerTime;
	vector<float> monsterTime;	//���߿� �������� ������!
	vector<float> rangeMonsterTime;	//Ȱ ����
	float timing = 0.0f;
	
private:
	//�迭
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

	//�¾����� �ѹ��� �°�
	bool monsterToPlayer[MONSTERCOUNT + RANGEMONSTERCOUNT]{ false };	//���Ͱ� �÷��̾� ����
	bool playerToMonster[MONSTERCOUNT + RANGEMONSTERCOUNT]{ false };	//�÷��̾ ���� ����

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
#pragma once
#include "./GameObjects/GameObject.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	void Update() override;
	void Render() override;

	void Fire(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	void Stop();
	void BallShootPos(int ball) { ballShootpos = ball; }
	void PlayerXPos(Vector3 ball) { playerXPos = ball; }
	void outhand(int hand) { outofhand = hand;}
	void BallBlocked(int ball) { ballBlocked = ball;}
	void Dunk(int ball) { if (ball == 2) bDunk = true; else false; }
	void IsBlocking(bool block) { bBlock = block; }
	bool IsFire() { return bFire; }
	bool IsBlocked() { return bBlock; }	//반환
	int outBall() { return gravityCount; }
	int outhanding() { return outofhand; }
	void noGoalgra(int i) { if (i == 1) noGoalgravity = 5.0f;  else if (i == 2) noGoalgravity = 1.0f; }
	class GameObject* GetObj() { return reinterpret_cast<GameObject*>(objx); }

private:
	class ObjectX* objx;
	bool bFire;
	bool bBlock;
	bool bDunk;
	bool ballControl; //ballShootpos의 반복문 컨트롤
	bool LayupControl; //Layup일 경우 true
	int outofhand;	//손밖에 공이 있을경우 0. 첫시작 1. 슛 2. 골대에서 튕김 3. 공소지 4. 공들어감 5. 레이업
	int noMove;
	int ballBlocked;	//공블락되고 튀기는곳 1.왼 2.오 3.왼대각 4.오대각 5.가운데
	int ballShootpos;	//공쏠때 세기를 정하기 위한 값
	float ballspin;	//공회전
	float noGoalgravity;	//노골일것 같을때 중력강화
	float ballAngle;
	float ballPowerX;	//볼의세기
	float ballPowerY;	//볼의세기
	float gravity;	//볼의 중력
	float gravityplus;	//볼의 중력추가
	float gravityMin;	//볼의 감소중력
	int gravityCount;	//볼이 몇번 튀기면 더이상 튀기지 않게

	Vector3 direction;
	Vector3 playerXPos;
	Vector2 xy;
	Vector3 orgin; //시작지점
	Vector3 pos;	//공위치
	Vector3 goalPos; //골대 위치
	Vector3 ballPos; //골대튀기는 높이위치
	Vector3 shootPos;
	Vector3 temp; //플레이어와 골대의 거리
	Vector3 objxSave;	//오브젝트 위치세이브
};
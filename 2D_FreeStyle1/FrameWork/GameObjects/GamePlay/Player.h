#pragma once
#include "./GameObjects/GameObject.h"
#include "./Systems/SingletonBase.h"

class Player : public GameObject , public SingletonBase<Player>, public CMemeoryPool<Player>
{
public:
	Player();
	~Player();

	void Update() override;
	void Render() override;
	void Dontmove(bool move) { dontmove = move; }
	bool Fake() { return fake; }	//페이크중인가 리턴
	int Nomove() { return noMove; }	//슛중일때 타임리미트일때는 계속하게해줌
	void JumpControl(int frame); //리미트점프
	void JumpShoot(int pos);	//점프슛
	void LayupDunk(int pos);	//레이업과 덩크슛
	int BlockBallDirection() { return blockDirection; }	//블록후 공튕기는 위치
	void Block();	//블락
	bool Blocked() { return blocked; }	//블락 송출
	bool BlockMotion() { return block; }	//블락중일때 송출
	bool Blocking() { return blocking; }	//블락가능을 송출
	Vector3 BlockPos() { return blockPos; }	//블락위치 송출
	bool NoColi() { return noColi; }	//충돌방지 송출
	int Dunkball() { return LayOrDunk; } //덩크일 때 공의 처리를 위해서 t/f 넘기기
	void SlideLayDunk(); //레이업의 골을 넣은후 미끄러지듯이 더 앞쪽으로 넘어감
	void Breakthrough(); //돌파
	void Stopbreakth();	//돌파시 부딫히면 멈추게

	int LeRiPos() { return iLeRiPos; }//위치가 왼쪽인가 오른쪽인가 1왼 2오

	Vector3 Shoot(); //슛위치값 넘겨주기
	void IsShoot(bool shoot) { shooting = shoot; }//슈팅 하는중인가
	bool IsShooting() { return shooting; } //반환용
	void IsHand(bool hand) { handBall = hand;}
	bool IsHanding() { return handBall; } //반환용
	bool IsbreakthColing() { return breaker; } //반환용
	void noChangeFalse(){ for (int i = 0; i < 5; i++) noChange[i] = false; }
	class GameObject* GetObj() { return reinterpret_cast<GameObject*>(objX); }

		
private:
	class ObjectX* objX;
	class p1color* p1;
	class Animator* anim;
	class Audio* jumpShootSound;
	class Audio* CvDunk;

	float speed;	//애니메이션속도
	float mSpeed;	//움직이는속도
	float shootSpeed;	//슛모션속도
	float hJump;	//점프높이
	float LayUpSpeed; //레이업 속도
	float breakthSpeed; //돌파 속도
	bool dontmove;	//움직이지않게
	bool diago;	//대각선 판단
	bool handBall; //공있나 판단
	bool shooting;	//슛타이밍 판단
	bool noStop; //슛쏠때 동작이 안멈추게
	bool objxFix;	//objxFix를 고정
	bool playmove;	//안 움직이는 곳 움직이게
	bool fixi;	//개빡친
	bool breaker;	//돌파
	bool block;	//블락
	bool blocked;	//블락타이밍
	bool blocking;	//슛들의 블락타이밍
	bool noChange[5];	//동작중 다른 모션으로 바뀌는것 방지
	bool noColi;	//동작중 밀림방지
	bool fake;	//페이크일때는 못밀게
	int blockDirection; //블락위치	//1.중앙 2.오른 3.왼
	int LayOrDunk;	//1.레이업 2.덩크
	int noMove;	//동작중 움직임방지
	int noChangein;	//0.왼쪽사이드 1.오른사이드 2.왼쪽대각 3.오른대각 4. 중앙
	int limitJump; //프레임이 빨라도 점프 정위치에서 작동
	int posShoot; //슛한위치	1.사이드 2.대각선 3.중앙
	int iLeRiPos; //왼쪽 오른쪽
	int JumpOrLay; //점프 제어할때 점프슛인가 레이업인가 1.jump 2.layup
	int breakth;	//돌파 왼쪽사이드 1.왼 2.오 오른사이드 3.왼 4.오 
	int breakth2;	//돌파위치에서의 속도 제어


	Vector3 vec;
	Vector3 pos2;
	Vector3 pos2Save;  
	Vector3 objXPos;
	Vector3 scaleSave;
	Vector3 adjScale;
	Vector3 offset; 
	Vector3 breakpos;
	Vector3 FixPos;
	Vector3 blockPos;
};


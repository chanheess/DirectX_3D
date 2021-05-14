#pragma once
#include "Scene.h"
class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	virtual void Ready() override;
	virtual void Update() override;
	virtual void Render() override;
private:
	Vector3 vec;
	Vector3 GoalInPos;
	Vector3 playerPos;
	Vector3 playerPos2;
	Vector3 playerObjxPos;
	Vector3 playerObjxPos2;
	Vector3 ballPos;
	Vector3 ballObjxPos;
	Vector3 enemyPos;

	Vector2 P1LeftUpColi;
	Vector2 P1LeftDownColi;
	Vector2 P1RightUpColi;
	Vector2 P1RightDownColi;
	Vector2 P2LeftUpColi;
	Vector2 P2LeftDownColi;
	Vector2 P2RightUpColi;
	Vector2 P2RightDownColi;

	int randomShoot[100];	//슛확률
	int player1Point;	//점수
	int player2Point;	//점수
	bool p1Shoot;	//누가 슛을쐇나
	bool p2Shoot;
	float nowTime;		//현재 시간
	float startTime;	//시작 시간
	float playTime;		//경기시작 플레이타임
	float playCountTime;	//경기시작 남은시간
	float gameSet;	//경기종료
	bool gameSetting;	//경기종료
	float delayTime;	//게임 끝 후에 장면변환시간
	bool timeStopper;	//시간 멈추기
	bool onemore;	//점수한번만오르기
	bool goalanddnogoal;	//골대맞거나 팅기면 on
	bool noReplay; //두번이상재생안되게
	
	class MapBackground* map;

	//클래스에 묶어도 될듯
	class Goaldae2* frontGoaldae;
	class Goaldae* backGoaldae;

	class Ball* ball;
	class ObjectX* objX;

	//클래스에 묶어도 될듯
	class Audio* endplay;
	class Audio* countdown;
	class Audio* rimout;
	class Audio* Block;
	class Audio* CvBlock;
	class Audio* Dunk;
	class Audio* GoalIn;
	class Audio* NoGoal;


	//클래스에 묶어도 될듯
	class score* scoreboard;
	class score* start;
	class score* win[2];
	class score* scoreNum[11];
	class score* scoreNum2[11];
	class score* timeNum[11];
};


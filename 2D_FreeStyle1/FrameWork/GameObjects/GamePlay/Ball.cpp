#include "stdafx.h"
#include "Ball.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Animator.h"
#include "./Components/Animation/Clip.h"
#include "./Components/Animation/Sprite.h"
#include "./Components/Bounding/BBox.h"
#include "./GameObjects/GamePlay/ObjectX.h"

Ball::Ball()
	: GameObject(L"ball"), bFire(false), noMove(0), xy(0,0),
	goalPos(638,1000,0), shootPos(638, 550, 0), gravity(0.0f),
	pos(0,0,0), outofhand(0), gravityMin(1.0f),
	gravityCount(0), ballPowerX(0), ballPowerY(0), noGoalgravity(1.0f)
	, ballspin(0), ballControl(false), LayupControl(false), bDunk(false)
	, ballBlocked(0), bBlock(false)
	//y 590골대	//1000슛궤도위치
{
	objx = new ObjectX();
	AddComponent(new BBox);
	transform->Pivot(D3DXVECTOR2(0.5f, 0.5f));
	transform->Scale(Vector3(20, 20, 0));
	transform->Position(638, 350);
	AddComponent(new Sprite(L"ball.png", 2, 2, 97, 97));
	
	gravityplus = 0.02f;
}
Ball::~Ball() 
{
}

void Ball::Update()
{
	if (outofhand == 3) return;
	__super::Update();
	
	objx->Update();
	//공회전
	ballspin -= 1.0f;
	transform->Rotation(Math::ToRadian(ballspin));
	//==
	if (bFire == true && outofhand == 1)
		objx->Pos(pos, 1);
	pos = transform->Position();
	//==========================공 맵밖으로 안나가게
	{	//p1
		if (pos.y >= 900)
		{
			pos.y -= 1.0f;
			transform->Position(pos);
		}
		if (pos.y <= 25.0f)
		{
			pos.y = 25.0f;
			transform->Position(pos);
		}
		if (pos.x <= 20.0f)
		{
			pos.x = 20.0f;
			transform->Position(pos);
		}
		if (pos.x >= 1250.0f)
		{
			pos.x = 1250.0f;
			transform->Position(pos);
		}
	}
	//==========================점프볼=======================
	if (outofhand == 0)
	{
		bFire = false;
		objxSave = Vector3(638, 315, 0);
		objx->Pos(objxSave, 3);
		if (gravityCount < 6)
		{
			temp = goalPos - pos;
			D3DXVec3Normalize(&temp, &temp);
			temp.y += g_ETime;
			pos += temp;
			pos.y += 1.8f;
			if (objxSave.y > pos.y)
			{
				if (goalPos.y > objxSave.y)
				{
					gravityMin += 2.0f;
					gravityCount += 1.0f;
				}
				gravity = 0.0f;
			}
			else {
				if (goalPos.y != objxSave.y)
				{
					gravity += 0.01f * gravityMin;
					pos.y -= gravity;
				}
			}
		}
		transform->Position(pos);
	}
	//===========================슛==========================
	if (outofhand == 1)
	{
		//사이드
		if (ballShootpos == 1 && ballControl == false)
		{
			//골대와 거리가 멀수록 볼파워증가
			if ((D3DXVec3Length(&(playerXPos - goalPos)) > 700.0f))
			{
				ballPowerX = 1.4f;
				ballPowerY = 1.4f;
			}
			else if ((D3DXVec3Length(&(playerXPos - goalPos)) > 675.0f))
			{
				ballPowerX = 1.4f;
				ballPowerY = 1.3f;
			}
			else if ((D3DXVec3Length(&(playerXPos - goalPos)) > 650.0f))
			{
				ballPowerX = 1.3f;
				ballPowerY = 1.2f;
			}
			else if ((D3DXVec3Length(&(playerXPos - goalPos)) > 625.0f))
			{
				ballPowerX = 1.2f;
				ballPowerY = 1.1f;
			}
			else if ((D3DXVec3Length(&(playerXPos - goalPos)) > 600.0f))
			{
				ballPowerX = 1.0f;
				ballPowerY = 1.0f;
			}
			else if ((D3DXVec3Length(&(playerXPos - goalPos)) > 550.0f))
			{
				ballPowerX = 0.85f;
				ballPowerY = 0.85f;
			}
			ballControl = true;
		}
		//대각선
		else if (ballShootpos == 2 && ballControl == false)
		{
			float xdis = playerXPos.x - goalPos.x;
			float ydis = 0;
			ydis = 400.0f - playerXPos.y;

			if (xdis <= 0)
				xdis = xdis * (-1);

			//골대와 거리가 멀수록 볼파워증가
			{
				//xxxxxxxxxxxxxxxxxxxxxxxxxxx
				{
					if (xdis >= 608.0f)
					{
						ballPowerX = 1.8f;
						ballPowerY = 1.8f;
					}
					else if (xdis >= 588.0f)
					{
						ballPowerX = 1.8f;
						ballPowerY = 1.8f;
					}
					else if (xdis >= 568.0f)
					{
						ballPowerX = 1.7f;
						ballPowerY = 1.8f;
					}
					else if (xdis >= 548.0f)
					{
						ballPowerX = 1.7f;
						ballPowerY = 1.8f;
					}
					else if (xdis >= 528.0f)	//110~90
					{
						ballPowerX = 1.7f;
						ballPowerY = 1.7f;
					}
					else if (xdis >= 508.0f)	//130~110
					{
						ballPowerX = 1.6f;
						ballPowerY = 1.7f;
					}
					else if (xdis >= 488.0f)	//150~130
					{
						ballPowerX = 1.6f;
						ballPowerY = 1.7f;
					}
					else if (xdis >= 468.0f)	//170~150
					{
						ballPowerX = 1.6f;
						ballPowerY = 1.7f;
					}
					else if (xdis >= 448.0f)	//190~170
					{
						ballPowerX = 1.6f;
						ballPowerY = 1.7f;
					}
					else if (xdis >= 428.0f)	//210~190
					{
						ballPowerX = 1.6f;
						ballPowerY = 1.6f;
					}
					else if (xdis >= 408.0f)	//230~210
					{
						ballPowerX = 1.6f;
						ballPowerY = 1.6f;
					}
					else if (xdis >= 388.0f)	//250~230
					{
						ballPowerX = 1.5f;
						ballPowerY = 1.5f;
					}
					else if (xdis >= 368.0f)	//270~250
					{
						ballPowerX = 1.4f;
						ballPowerY = 1.5f;
					}
					else if (xdis >= 348.0f)	//290~270
					{
						ballPowerX = 1.4f;
						ballPowerY = 1.4f;
					}
					else if (xdis >= 328.0f)	//310~290
					{
						ballPowerX = 1.3f;
						ballPowerY = 1.4f;
					}
					else if (xdis >= 308.0f)	//330~310
					{
						ballPowerX = 1.3f;
						ballPowerY = 1.4f;
					}
					else if (xdis >= 288.0f)	//350~330
					{
						ballPowerX = 1.2f;
						ballPowerY = 1.4f;
					}
					else if (xdis >= 268.0f)	//370~350
					{
						ballPowerX = 1.2f;
						ballPowerY = 1.3f;
					}
					else if (xdis >= 248.0f)	//390~370
					{
						ballPowerX = 1.1f;
						ballPowerY = 1.3f;
					}
					else if (xdis >= 228.0f)	//410~390
					{
						ballPowerX = 1.0f;
						ballPowerY = 1.2f;
					}
					else if (xdis >= 208.0f)	//430~410
					{
						ballPowerX = 0.9f;
						ballPowerY = 1.2f;
					}
					else if (xdis >= 188.0f)	//188은 골대638에서 슛시작점 450의 뺀값인 사이값을 시작점으로
					{							//곧 188이 0점인셈
						ballPowerX = 0.8f;
						ballPowerY = 1.1f;
					}
				}
				//yyyyyyyyyyyyyyyyyyyyyyyyyyy
				{
					float i = 400.0f;	//y의 시작값
					float j = 2.08f;	//y파워의 최대값 -> 가까울수록 점점 줄어들음
					while (1)
					{
						if (ydis >= i)
						{
							ballPowerY += j;
							break;
						}
						else
						{
							j -= 0.05f;
							i -= 10.0f;
						}
					}
				}
				if (playerXPos.y <= 100.0f)
				{
					ballPowerY += 0.05f;
					ballPowerX -= 0.2f;
				}
			}
			ballControl = true;
		}
		//중앙
		else if (ballShootpos == 3 && ballControl == false)
		{
			float xdis = playerXPos.x - goalPos.x;
			if (xdis <= 0)
				xdis = xdis * (-1);
			float ydis = 0;
			ydis = 245.0f - playerXPos.y;
			ballPowerX = 0.0f;
			ballPowerY = 0.0f;
			if (xdis >= 178.0f)
				ballPowerX = 0.63f;
			else if (xdis >= 158.0f)
				ballPowerX = 0.58f;
			else if (xdis >= 138.0f)
				ballPowerX = 0.53f;
			else if (xdis >= 118.0f)
				ballPowerX = 0.48f;
			else if (xdis >= 98.0f)
				ballPowerX = 0.43f;
			else if (xdis >= 78.0f)
				ballPowerX = 0.38f;
			else if (xdis >= 58.0f)
				ballPowerX = 0.33f;
			else if (xdis >= 38.0f)
				ballPowerX = 0.28f;
			else if (xdis >= 18.0f)
				ballPowerX = 0.23f;
			float j = 0;	//y의 시작값
			float i = 2.05f;	//y의 시작값
			while (1)
			{
				if (ydis <= j)
				{
					ballPowerY += i;
					break;
				}
				else {
					j += 10.0f;
					i += 0.05f;
				}
			}
			ballControl = true;
		}
		temp = goalPos - pos;
		D3DXVec3Normalize(&temp, &temp);
		temp.x += g_ETime;
		temp.y += g_ETime;
		pos += temp;
		//왼쪽 오른쪽에 따라서 볼파워 결정
		if (playerXPos.x < 638)
			pos.x += ballPowerX;
		if (playerXPos.x >= 638)
			pos.x -= ballPowerX;
		pos.y += ballPowerY;
		if (ballShootpos == 1 || ballShootpos == 2)
			gravity += gravityplus * noGoalgravity;
		else if (ballShootpos == 3)
			gravity += gravityplus;
		pos.y -= gravity;
		transform->Position(pos);
	}
	//===================골대에서 튕겼을때====================
	if (outofhand == 2)
	{
		bFire = false;
		if (gravityCount == 0)
		{
			srand((UINT)time(NULL));
			if (LayupControl == false)
			{
				int x = rand() % 476;
				int y = rand() % 225;
				objxSave = Vector3((x + 400.0f), (y + 265.0f), 0);
			}
			else if (LayupControl == true)
			{
				int x = rand() % 156;
				int y = rand() % 70;
				objxSave = Vector3((x + 560.0f), (y + 360.0f), 0);
				LayupControl = false;
			}
			objx->Pos(objxSave, 3);
			ballPos.x = objxSave.x;
			ballPos.y = 750.0f;
			ballPos.z = 0.0f;
			gravityCount = 1;
			gravity = 0.0f;
			gravityMin = 1.0f;
		}
		if (gravityCount < 10)
		{
			temp = ballPos - pos;
			D3DXVec3Normalize(&temp, &temp);
			temp.x += g_ETime;
			temp.y += g_ETime;
			pos += temp;

			if (objxSave.y > pos.y)
			{
				if (ballPos.y > objxSave.y)
				{
					gravityMin += 1.5f;
					gravityCount++;
				}
				gravity = 0.0f;
			}
			else {
				if (ballPos.y != objxSave.y)
				{
					gravity += 0.01f * gravityMin;
					pos.y -= gravity;
				}
			}
		}
		transform->Position(pos);
	}
	//===========================골==========================
	if (outofhand == 4)
	{
		bFire = false;
		if (gravityCount == 0)
		{
			objxSave = Vector3(638.0f, 475.0f, 0);
			objx->Pos(objxSave, 3);
			ballPos.x = objxSave.x;
			ballPos.y = 750.0f;
			ballPos.z = 0.0f;
			gravityCount = 1;
			gravityMin = 1.0f;
			if (LayupControl == false)
			{
				if (pos.x >= 628.0f && pos.x <= 648.0f)
					gravity = 1.5f;
				else if (pos.x <= 628.0f && pos.x >= 613.0f)
					gravity = 0.3f;
				else if (pos.x >= 648.0f && pos.x <= 663.0f)
					gravity = 0.3f;
			}
			else if (LayupControl == true)
			{
				if (bDunk == true)
				{
					gravity = 3.0f;
					bDunk = false;
				}
				else
				{
					if (pos.x >= 628.0f && pos.x <= 648.0f)
						gravity = 1.5f;
					else
						gravity = 0.5f;
				}
				LayupControl = false;
			}
		}
		else if (gravityCount < 6)
		{
			temp = ballPos - pos;
			D3DXVec3Normalize(&temp, &temp);
			temp.x += g_ETime;
			temp.y += g_ETime;
			pos += temp;
			if (objxSave.y > pos.y)
			{
				if (645.0f > objxSave.y)
				{
					gravityMin += 1.5f;
					gravityCount += 1.0f;
				}
				gravity = 0.0f;
			}
			else {
				if (645.0f != objxSave.y)
				{
					gravity += 0.01f * gravityMin;
					pos.y -= gravity;
				}
			}
		}
		transform->Position(pos);
	}
	//=======================레이업==========================
	if (outofhand == 5)
	{
		LayupControl = true;
		if (bDunk == true)
			pos.x = 638, pos.y = 655;
		else {
			temp = goalPos - pos;
			D3DXVec3Normalize(&temp, &temp);
			temp.x += g_ETime;
			temp.y += g_ETime;
			pos += temp;
			gravity += 0.01f;
			pos.y -= gravity;

		}
		transform->Position(pos);
	}
	//=========================블락==========================
	if (outofhand == 6)
	{
		bFire = false;
		if (gravityCount == 0)
		{
			srand((UINT)time(NULL));
			int x = rand() % 100;
			switch (ballBlocked)
			{
			case 1:
				objxSave = Vector3((pos.x - (x + 400)), (pos.y - 200), 0);
				break;
			case 2:
				objxSave = Vector3((pos.x + (x + 400)), (pos.y - 200), 0);
				break;
			case 3:
				objxSave = Vector3((pos.x - (x + 100)), (pos.y - 300), 0);
				break;
			case 4:
				objxSave = Vector3((pos.x + (x + 100)), (pos.y - 300), 0);
				break;
			case 5:
				objxSave = Vector3(((pos.x - 100) + x), (pos.y - 400), 0);
				break;
			}
			objx->Pos(objxSave, 3);
			ballPos.x = objxSave.x;
			ballPos.y = pos.y + 200;
			ballPos.z = 0.0f;
			gravityCount = 1;
			gravity = 2.0f;
			gravityMin = 1.0f;
		}
		if (gravityCount < 10)
		{
			temp = ballPos - pos;
			D3DXVec3Normalize(&temp, &temp);
			temp.x += g_ETime;
			temp.y += g_ETime;
			pos += temp;
			switch (ballBlocked)
			{
			case 1:
				pos.x -= 2.0f;
				break;
			case 2:
				pos.x += 2.0f;
				break;
			case 3:
				pos.x -= 1.0f;
				break;
			case 4:
				pos.x += 1.0f;
				break;
			}
			if (objxSave.y > pos.y)
			{
				if (ballPos.y > objxSave.y)
				{
					gravityMin += 1.5f;
					gravityCount++;
				}
				gravity = 0.0f;
			}
			else {
				if (ballPos.y != objxSave.y)
				{
					gravity += 0.01f * gravityMin;
					pos.y -= gravity;
				}
			}
		}
		transform->Position(pos);
	}
	if (pos.y <= 26.0f)
	{
		gravityCount = 33;
	}
}
void Ball::Render()
{
	if (outofhand == 3) return;
	if(outofhand != 1)
		objx->Render();
	__super::Render();
	/*ImGui::Begin("Ball");
	{
		ImGui::Text("gravity : %.2f", gravity);
		ImGui::Text("ballpos : X:%.2f Y:%.2f", pos.x, pos.y);
		ImGui::Text("ballPower : X:%.2f Y:%.2f", ballPowerX, ballPowerY);
	}
	ImGui::End();*/
}

void Ball::Fire(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	bFire = true;
	transform->Position(pos);
	orgin = pos;
	direction = dir;
	ballAngle = acosf(D3DXVec3Length(&dir) / dir.x);
	transform->Rotation(ballAngle);
	gravity = 0.0f;
	gravityCount = 0;
	ballControl = false;
}

void Ball::Stop()
{
	bFire = false;
}

#include "Framework.h"
#include "ModelControler.h"

ModelControler::ModelControler()
{
	move = Vector3(0, 0, 0);
	arrival = Vector3(-5, 0, -5);

	position1 = Vector3(0, 0, 0);
	rotation1 = Vector3(0, 0, 0);

	movePosition[0] = Vector3(0, 0, 0);
	movePosition[1] = Vector3(0, 0, 0);
	movePosition[2] = Vector3(0, 0, 0);
	movePosition[3] = Vector3(0, 0, 0);

	savePosition = Vector3(0, 0, 0);
	tempPosition = Vector3(0, 0, 0);
	randomPosition = Vector2(0, 0);

	patrolState = PatrolState::pIdle;

	angle = 0;
	currAngle = 0;
	prevAngle = 0;
	autoAttack = 0;

	srand((UINT)time(0));
}

ModelControler::~ModelControler()
{

}

void ModelControler::Patrol(Transform* pos, Vector3 pos2, bool input, float posY)
{
	patrolScale = 30.0f;
	pos->Position(&tempPosition);

	if (randomOn == false)
	{
		int random = 0;
		random = rand() % 8;

		savePosition = tempPosition;
		{
			//East
			movePosition[0].x = savePosition.x + patrolScale;
			movePosition[0].z = savePosition.z;
			movePosition[0].y = savePosition.y;
			//South
			movePosition[1].x = savePosition.x;
			movePosition[1].z = savePosition.z - patrolScale;
			movePosition[1].y = savePosition.y;
			//West
			movePosition[2].x = savePosition.x - patrolScale;
			movePosition[2].z = savePosition.z;
			movePosition[2].y = savePosition.y;
			//North
			movePosition[3].x = savePosition.x;
			movePosition[3].z = savePosition.z + patrolScale;
			movePosition[3].y = savePosition.y;
		}
		switch (random)
		{
		case 0:
			randomPos[0] = 0;
			randomPos[1] = 1;
			randomPos[2] = 2;
			randomPos[3] = 3;
			break;
		case 1:
			randomPos[0] = 1;
			randomPos[1] = 2;
			randomPos[2] = 3;
			randomPos[3] = 0;
			break;
		case 2:
			randomPos[0] = 2;
			randomPos[1] = 3;
			randomPos[2] = 0;
			randomPos[3] = 1;
			break;
		case 3:
			randomPos[0] = 3;
			randomPos[1] = 0;
			randomPos[2] = 1;
			randomPos[3] = 2;
			break;
		case 4:
			randomPos[0] = 3;
			randomPos[1] = 2;
			randomPos[2] = 1;
			randomPos[3] = 0;
			break;
		case 5:
			randomPos[0] = 2;
			randomPos[1] = 1;
			randomPos[2] = 0;
			randomPos[3] = 3;
			break;
		case 6:
			randomPos[0] = 1;
			randomPos[1] = 0;
			randomPos[2] = 3;
			randomPos[3] = 2;
			break;
		case 7:
			randomPos[0] = 0;
			randomPos[1] = 3;
			randomPos[2] = 2;
			randomPos[3] = 1;
			break;
		}

		randomOn = true;
	}
	if (randomOn == true)
	{
		if (state != AnimationState::Death)
		{
			//1 ������ġ 2 �÷��̾���ġ 3ù������ġ
			autoAttack = PatrolAutoAttack(tempPosition, pos2, savePosition, input);

			switch (autoAttack)
			{
			case 1:	//�÷��̾� ���󰡱�
				MoveOn = true;
				Move(pos, pos2, true, 10, posY);
				break;
			case 2:	//������
				if (input == false)
				{
					if (state != AnimationState::PunchAttack)
						state = AnimationState::PunchAttack;	//������
				}
				else if (input == true)
				{
					if (state != AnimationState::DistanceAttack)
						state = AnimationState::DistanceAttack;	//������
				}
				MoveOn = true;
				break;
			case 3:	//����
				if (MoveOn == true) //������ �Ŀ��� �����̰�
					Move(pos, movePosition[randomPos[patrolNum]], true, 10, posY);

				if (patrolState == PatrolState::pIdle)
				{
					MoveOn = false;
					patrolTime += Time::Get()->Delta();
					if (patrolTime >= 3.0f)	// ������ �� ������ �� ������
					{
						patrolTime = 0;
						patrolNum++;
						patrolNum %= 4;
						MoveOn = true;
					}
				}
				break;

			case 4:	//����������Ż
				MoveOn = true;
				Move(pos, savePosition, true, 10, posY);

				if (patrolState == PatrolState::pIdle)
				{
					outRange = false;
					lockOn = false;
				}
				break;
			}
		}//if (state != AnimationState::Death)
	}
}

UINT ModelControler::PatrolAutoAttack(Vector3 pos, Vector3 pos2, Vector3 savepos, bool input)
{
	//�Ÿ�
	float playerDis = sqrt((pos2.z - pos.z) * (pos2.z - pos.z) +
		(pos2.x - pos.x) * (pos2.x - pos.x));

	float saveDis = sqrt((savepos.z - pos.z) * (savepos.z - pos.z) +
		(savepos.x - pos.x) * (savepos.x - pos.x));

	//���� ����
	float sensor = 25.0f;
	//���� ����
	float attackRange;

	if (input == false)
		attackRange = 10.0f;
	if (input == true)
		attackRange = 30.0f;

	//patrolScale������ �������ִ� ��� ����
	float possibility = sensor + patrolScale;

	//�÷��̾ �������� �ٱ��� �����������
	if (saveDis >= possibility)
	{
		outRange = true;
	}

	//������ ������ ��� �������� �ȿ��� �÷��̾� ���󰡱�
	if (playerDis < sensor)	
	{
		lockOn = true;
	}
	//���� ��ġ�� ���ư���.
	if (outRange == true)
	{
		return 4;
	}
	//����
	else if (playerDis <= attackRange)
	{
		return 2;
	}
	else if (lockOn == true)
	{
		return 1;
	}
	else
	{
		return 3;
	}
	
}

void ModelControler::Attack(UINT attack, Transform* pos, Vector3 mousePos, bool turn)
{
	switch(attack)
	{
	case 1:	//Sword
		state = AnimationState::SwordAttack;
		break;
	case 2:	//Punch
		state = AnimationState::PunchAttack;
		break;
	case 3:	//Range
		state = AnimationState::RangeAttack;
		break;
	case 4:	//Distance
		state = AnimationState::DistanceAttack;
		break;
	}

	if (turn == true && moveMode == false)
	{
		pos->RotationDegree(&rotation1);

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = mousePos - position1;

		Vector3 normal3;
		normal3 = -pos->Right();

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);
		D3DXVec3Normalize(&normal3, &normal3);

		angle = D3DXVec3Dot(&normal1, &normal2);

		float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
		angle3_2 = acosf(angle3_2);
		angle3_2 = Math::ToDegree(angle3_2);

		if (angle3_2 > 90)
		{
			angle = -acosf(angle);
			angle = Math::ToDegree(angle);
		}
		else if (angle3_2 <= 90)
		{
			angle = acosf(angle);
			angle = Math::ToDegree(angle);
		}

		prevAngle = rotation1.y;
		currAngle = rotation1.y + angle;

		if ((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
			rotation1.y += angle;

		pos->RotationDegree(rotation1);
	}
	else if (turn == true && moveMode == true)
	{
		pos->RotationDegree(&rotation1);

		//ĳ������ ī�޶��� ���⿡ ���ߴ� ������ȯ
		Vector3 rotation;
		((Freedom *)Context::Get()->GetCamera())->RotationDegree(&rotation);

		rotation1.y = rotation.y - 180.0f;
		pos->RotationDegree(rotation1);
	}
}

void ModelControler::Dash(Transform* pos, Vector3 pos2, bool input)
{
	pos->Position(&position1);
	pos->RotationDegree(&rotation1);

	//move
	if (input == true)
	{
		state = AnimationState::Dashed;

		//move2
		if (moveMode == true)
			pos2 = position1 - pos->Forward() * 40.0f;

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = pos2 - position1;

		Vector3 normal3;
		normal3 = -pos->Right();

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);
		D3DXVec3Normalize(&normal3, &normal3);

		angle = D3DXVec3Dot(&normal1, &normal2);

		float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
		angle3_2 = acosf(angle3_2);
		angle3_2 = Math::ToDegree(angle3_2);

		if (angle3_2 > 90)
		{
			angle = -acosf(angle);
			angle = Math::ToDegree(angle);
		}
		else if (angle3_2 <= 90)
		{
			angle = acosf(angle);
			angle = Math::ToDegree(angle);
		}

		prevAngle = rotation1.y;
		currAngle = rotation1.y + angle;

		if ((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
			rotation1.y += angle;

		//������ ����
		arrival = position1 + normal2 * 40.0f;

		move = normal2 * 0.4f;
	}

	//ArriveDistance
	dashDistance = sqrt((arrival.z - position1.z) * (arrival.z - position1.z) +
		(arrival.x - position1.x) * (arrival.x - position1.x));


	if (state == AnimationState::Dashed)
	{
		if (arrival != Vector3(-5, 0, -5) && (dashDistance < 0.15f) != true)
		{
			position1 += move;
		}

		else {
			
		}
	}

	pos->RotationDegree(rotation1);
	pos->Position(position1);
}

void ModelControler::DistanceAttacked(Transform* pos, Vector3 pos2)	//state�� ������� �׳� ȭ������
{
	pos->Position(&position2);

	if (arrowState == ArrowState::ArrowReload)
	{
		firstArrow = position2;

		//move2
		if (moveMode == true)
		{
			arriveArrow = position2 - pos->Up() * 200.0f;
			arriveArrow.y = 0;
		}
		else if (moveMode == false)
		{
			arriveArrow = pos2;
			arriveArrow.y = 0;
		}

		Vector3 normal;
		normal = arriveArrow - firstArrow;

		D3DXVec3Normalize(&normal, &normal);

		//������ ����
		arrival = firstArrow + normal * 200.0f;

		moveArrow = normal * 0.8f;

		arrowState = ArrowState::ArrowShoot;
	}

	//ArriveDistance
	if (arrowState == ArrowState::ArrowShoot)
	{
		arrowDistance = sqrt((arrival.z - position2.z) * (arrival.z - position2.z) +
		(arrival.x - position2.x) * (arrival.x - position2.x));
	}


	if ((arrowDistance < 1.0f) != true && arrowState == ArrowState::ArrowShoot)
	{
		position2 += moveArrow;
	}
	else
	{
		if (arrowState != ArrowState::ArrowHit || ArrowState::ArrowShoot)
			arrowState = ArrowState::ArrowMiss;
	}

	pos->Position(position2);
}

void ModelControler::InputArrowState(UINT input)
{
	switch (input)
	{
	case 1:
		arrowState = ArrowState::ArrowIdle;
		break;
	case 2:
		arrowState = ArrowState::ArrowReload;
		break;
	case 3:
		arrowState = ArrowState::ArrowShoot;
		break;
	case 4:
		arrowState = ArrowState::ArrowHit;
		break;
	case 5:
		arrowState = ArrowState::ArrowMiss;
		break;
	}
}

void ModelControler::Move(Transform* pos, Vector3 pos2, bool patrol, UINT currEq, float posY)
{
	if (state == AnimationState::Dashed ||
		state == AnimationState::SwordAttack || 
		state == AnimationState::PunchAttack ||
		state == AnimationState::DistanceAttack ||
		state == AnimationState::RangeAttack)
	{
		return;
	}

	pos->Position(&position1);
	pos->RotationDegree(&rotation1);

	//�Ÿ���꿡 y���� �����ʰ�
	position1.y = 0.0f;

	if (patrol == false)
	{
		if (Mouse::Get()->Press(0))
		{
			//������ ����
			arrival = pos2;

			Vector3 normal1;
			normal1 = -pos->Forward();

			Vector3 normal2;
			normal2 = pos2 - position1;

			Vector3 normal3;
			normal3 = -pos->Right();

			D3DXVec3Normalize(&normal1, &normal1);
			D3DXVec3Normalize(&normal2, &normal2);
			D3DXVec3Normalize(&normal3, &normal3);

			angle = D3DXVec3Dot(&normal1, &normal2);

			float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
			angle3_2 = acosf(angle3_2);
			angle3_2 = Math::ToDegree(angle3_2);

			if (angle3_2 > 90)
			{
				angle = -acosf(angle);
				angle = Math::ToDegree(angle);
			}
			else if (angle3_2 <= 90)
			{
				angle = acosf(angle);
				angle = Math::ToDegree(angle);
			}

			prevAngle = rotation1.y;
			currAngle = rotation1.y + angle;

			if((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
				rotation1.y += angle;
		}
	}
	else if (patrol == true)
	{
		//������ ����
		arrival = pos2;

		Vector3 normal1;
		normal1 = -pos->Forward();

		Vector3 normal2;
		normal2 = pos2 - position1;

		Vector3 normal3;
		normal3 = -pos->Right();

		D3DXVec3Normalize(&normal1, &normal1);
		D3DXVec3Normalize(&normal2, &normal2);
		D3DXVec3Normalize(&normal3, &normal3);

		float angle = D3DXVec3Dot(&normal1, &normal2);

		float angle3_2 = D3DXVec3Dot(&normal3, &normal2);
		angle3_2 = acosf(angle3_2);
		angle3_2 = Math::ToDegree(angle3_2);

		if (angle3_2 > 90)
		{
			angle = -acosf(angle);
			angle = Math::ToDegree(angle);
		}
		else if (angle3_2 <= 90)
		{
			angle = acosf(angle);
			angle = Math::ToDegree(angle);
		}

		prevAngle = rotation1.y;
		currAngle = rotation1.y + angle;

		if ((currAngle > prevAngle + 0.5f || currAngle < prevAngle - 0.5f) == true)
			rotation1.y += angle;
	}
	
	if (patrol == false)
	{
		if (currEq == 0)
			move = pos->Forward() * 400.0f * Time::Get()->Delta();
		else if (currEq == 1)
			move = pos->Forward() * 250.0f * Time::Get()->Delta();
		else if (currEq == 2)
			move = pos->Forward() * 300.0f * Time::Get()->Delta();

	}
	else if (patrol == true)
	{
		if(autoAttack != 4)
			move = pos->Forward() * 100.0f * Time::Get()->Delta();
		else if(autoAttack == 4)
			move = pos->Forward() * 250.0f * Time::Get()->Delta();
	}

	//ArriveDistance
	distance = sqrt((arrival.z - position1.z) * (arrival.z - position1.z) +
		(arrival.x - position1.x) * (arrival.x - position1.x));

	if (arrival != Vector3(-5, 0, -5) && (distance < 0.15f) != true)
	{
		position1 -= move;

		if (patrol == false)
		{
			if (currEq == 0)
				state = AnimationState::Running;
			else
				state = AnimationState::Moving;
		}

		if (patrol == true)
		{
			state = AnimationState::Moving;
			patrolState = PatrolState::pMoving;
		}
	}
	else
	{
		if(state != AnimationState::SwordAttack ||
			state != AnimationState::PunchAttack ||
			state != AnimationState::DistanceAttack)
			state = AnimationState::Idle;

		if (patrol == true)
		{
			state = AnimationState::Idle;
			patrolState = PatrolState::pIdle;
		}
	}

	position1.y = posY;
	pos->RotationDegree(rotation1);
	pos->Position(position1);
}

void ModelControler::Move2(Transform * pos, UINT currEq, float posY)
{
	if (state == AnimationState::Dashed ||
		state == AnimationState::SwordAttack ||
		state == AnimationState::PunchAttack ||
		state == AnimationState::RangeAttack)
	{
		return;
	}

	pos->Position(&position1);
	pos->RotationDegree(&rotation1);

	//ĳ������ ī�޶��� ���⿡ ���ߴ� ������ȯ
	Vector3 rotation;
	((Freedom *)Context::Get()->GetCamera())->RotationDegree(&rotation);

	//������¿� ���� �������
	if (currEq == 0)
		state = AnimationState::Running;
	else if (state == AnimationState::DistanceAttack)
		state = AnimationState::DistanceAttack;
	else
		state = AnimationState::Moving;

	//������ȯ
	{
		if (Keyboard::Get()->Press('A'))
		{
			//bow move
			if (state == AnimationState::DistanceAttack)
			{
				//move speed and direction
				if (currEq == 2)
				{
					if (Keyboard::Get()->Press('W'))
						move = (-pos->Right() + pos->Forward()) * 230.0f * Time::Get()->Delta();
					else if (Keyboard::Get()->Press('S'))
						move = (-pos->Right() - pos->Forward()) * 230.0f * Time::Get()->Delta();
					else
						move = -pos->Right() * 280.0f * Time::Get()->Delta();
				}

				if (state == AnimationState::DistanceAttack)
					rotation1.y = rotation.y - 180.0f;
			}
			//defalut move
			else
			{
				//move speed and direction
				if (currEq == 0)
					move = pos->Forward() * 400.0f * Time::Get()->Delta();
				else if (currEq == 1)
					move = pos->Forward() * 250.0f * Time::Get()->Delta();
				else if (currEq == 2)
					move = pos->Forward() * 300.0f * Time::Get()->Delta();

				//move rotation
				if (Keyboard::Get()->Press('W'))
					rotation1.y = rotation.y - 235.0f;
				else if (Keyboard::Get()->Press('S'))
					rotation1.y = rotation.y - 315.0f;
				else
					rotation1.y = rotation.y - 270.0f;
			}
			
			position1 -= move;
		}
		else if (Keyboard::Get()->Press('D'))
		{
			//bow move
			if (state == AnimationState::DistanceAttack)
			{
				//move speed and direction
				if (currEq == 2)
				{
					if (Keyboard::Get()->Press('W'))
						move = (pos->Right() + pos->Forward()) * 230.0f * Time::Get()->Delta();
					else if (Keyboard::Get()->Press('S'))
						move = (pos->Right() - pos->Forward()) * 230.0f * Time::Get()->Delta();
					else
						move = pos->Right() * 280.0f * Time::Get()->Delta();
				}

				if (state == AnimationState::DistanceAttack)
					rotation1.y = rotation.y - 180.0f;
			}
			else
			{
				//move speed and direction
				if (currEq == 0)
					move = pos->Forward() * 400.0f * Time::Get()->Delta();
				else if (currEq == 1)
					move = pos->Forward() * 250.0f * Time::Get()->Delta();
				else if (currEq == 2)
					move = pos->Forward() * 300.0f * Time::Get()->Delta();

				//move rotation
				if (Keyboard::Get()->Press('W'))
					rotation1.y = rotation.y - 135.0f;
				else if (Keyboard::Get()->Press('S'))
					rotation1.y = rotation.y - 45.0f;
				else
					rotation1.y = rotation.y - 90.0f;
			}
			
			position1 -= move;
		}
		else if (Keyboard::Get()->Press('W'))
		{
			//bow move
			if (state == AnimationState::DistanceAttack)
			{
				//move speed and direction
				if (currEq == 2)
					move = pos->Forward() * 280.0f * Time::Get()->Delta();
			}
			//defalut move
			else
			{
				if (currEq == 0)
					move = pos->Forward() * 400.0f * Time::Get()->Delta();
				else if (currEq == 1)
					move = pos->Forward() * 250.0f * Time::Get()->Delta();
				else if (currEq == 2)
					move = pos->Forward() * 300.0f * Time::Get()->Delta();

			}

			rotation1.y = rotation.y - 180.0f;
			position1 -= move;
		}
		else if (Keyboard::Get()->Press('S'))
		{
			//bow move
			if (state == AnimationState::DistanceAttack)
			{
				//move speed and direction
				if (currEq == 2)
					move = -pos->Forward() * 280.0f * Time::Get()->Delta();

				if (state == AnimationState::DistanceAttack)
					rotation1.y = rotation.y - 180.0f;
			}
			else
			{
				if (currEq == 0)
					move = pos->Forward() * 400.0f * Time::Get()->Delta();
				else if (currEq == 1)
					move = pos->Forward() * 250.0f * Time::Get()->Delta();
				else if (currEq == 2)
					move = pos->Forward() * 300.0f * Time::Get()->Delta();

				rotation1.y = rotation.y;
			}

			position1 -= move;
		}
		else
		{
			if (state == AnimationState::DistanceAttack)
			{
				//defalut
			}
			//���ݾ��ϰ� ���������� ������ ���߱�
			else if (state != AnimationState::SwordAttack ||
				state != AnimationState::PunchAttack ||
				state != AnimationState::DistanceAttack)
				state = AnimationState::Idle;
		}
	}

	position1.y = posY;
	pos->RotationDegree(rotation1);
	pos->Position(position1);
}

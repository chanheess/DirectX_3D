#include "stdafx.h"
#include "Battle.h"
#include "Objects/HpBar.h"

void Battle::Initialize()
{
	//카메라의 초기 설정
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0 + 80, 32, -67 + 80);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"45_Scattering.fxo");
	shadow = new Shadow(shader, Vector3(0, 0, 0), 65);

	skyDome = new Sky(shader);
	skyDome->ScatteringPass(3);
	skyDome->Pass(4, 5, 6);
	skyDome->RealTime(false, Math::PI - 1e-6f);

	water = new Water(shader, 256);
	water->GetTransform()->Position(0, 1.5f, 0);


	//Terrain
	terrainShader = new Shader(L"47_TerrainLod.fxo");
	{
		TerrainLod::InitializeDesc desc =
		{
			terrainShader,
			L"Terrain/Gray512_Test.png",
			L"Terrain/Alpha512.png",
			1.0f, 16, 5
		};

		terrain = new TerrainLod(desc, false);
		terrain->BaseMap(L"Terrain/Dirt.png");
		terrain->LayerMap(L"Terrain/Grass (Lawn).jpg", 0);
		terrain->NormalMap(L"Terrain/Dirt_Normal.png");
		terrain->Pass(4);
		//terrain->Pass(1);
	}

	// 전투스타일 설정   false : topDownView / true : 3dview
	battleStyle = true;

	//시야스타일   false : topDownView / true : 3dview
	((Freedom *)Context::Get()->GetCamera())->BattleViewStyle(battleStyle);

	//Mesh();
	Player();
	Monster();
	NoAttachWeapons();
	AddSpotLights();

	explosion[0] = new ParticleSystem(L"ExplosionLayer1");
	explosion[1] = new ParticleSystem(L"ExplosionLayer2");
	explosion[2] = new ParticleSystem(L"ExplosionLayer3");
	explosion[3] = new ParticleSystem(L"ExplosionLayer4");

	playerHp = new HpBar(100, true);

	for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
		monsterHp[i] = new HpBar(100, false);



	//tree = new ModelRender(shader);
	//tree->ReadMaterial(L"Trees/Tree1");
	//tree->ReadMesh(L"Trees/Tree1");
	//
	//Transform* transform = NULL;
	//
	//transform = tree->AddTransform();
	//transform->Position(0,5,0);
	//transform->Scale(0.01f, 0.01f, 0.01f);
	//
	//models.push_back(tree);
	//
	//tree->UpdateTransforms();


}

void Battle::Destroy()
{
	SafeDelete(terrain);
	SafeDelete(playerHp);
	for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
		SafeDelete(monsterHp[i]);
}

void Battle::Update()
{
	if (isRender)
	{
		//캐릭터불러오기
		Vector3 pos;
		Vector3 rot;
		Vector3 sca;
		player->GetTransform(0)->Position(&pos);
		player->GetTransform(0)->Scale(&sca);
		player->GetTransform(0)->RotationDegree(&rot);

		//시야고정
		((Freedom *)Context::Get()->GetCamera())->LookAt(pos, sca.y);

		{
			//move
			if (playerControler[0]->MoveMode() == false && Mouse::Get()->Press(0) && playerControler[0]->AnimationState() != 9
				&& AttackState[0] != PlayerAttackState::RangeAttack && playerControler[0]->AnimationState() != 8)
			{
				Vector3 tempMousePos = terrain->GetPickedPosition();
				if (tempMousePos.z < 256.0f && tempMousePos.z > -256.0f &&
					tempMousePos.x < 256.0f && tempMousePos.x > -256.0f)
					mousePos = tempMousePos;

				if (player->CurrEquip() == 0)
				{
					playerControler[0]->RunStart();		//달리기모션
				}
				else
				{
					playerControler[0]->MoveStart();	//걷는모션
				}
			}
			//move2
			else if (playerControler[0]->MoveMode() == true && playerControler[0]->AnimationState() != 9
				&& AttackState[0] != PlayerAttackState::RangeAttack && playerControler[0]->AnimationState() != 8)
			{
				//키입력됐을때만 실행
				if (Keyboard::Get()->Press('A') || Keyboard::Get()->Press('D') ||
					Keyboard::Get()->Press('W') || Keyboard::Get()->Press('S'))
				{
					if (player->CurrEquip() == 0)
					{
						playerControler[0]->RunStart();		//달리기모션
					}
					else
					{
						playerControler[0]->MoveStart();	//걷는모션
					}
				}
			}
		}

		//WeaponChange
		if (playerControler[0]->AnimationState() != 8 && playerControler[0]->AnimationState() != 9 &&
			AttackState[0] == PlayerAttackState::Idle)
		{
			if (Keyboard::Get()->Down('1'))
			{
				player->Equipment(0);

				if (playerControler[0]->AnimationState() != 2)
					playerControler[0]->StopDash();
			}
			if (Keyboard::Get()->Down('2'))
			{
				if (getWeaponed[0] == true)
				{
					player->Equipment(1);

					if (playerControler[0]->AnimationState() != 2)
						playerControler[0]->StopDash();
				}
			}
			if (Keyboard::Get()->Down('3'))
			{
				if (getWeaponed[1] == true)
				{
					player->Equipment(2);

					if (playerControler[0]->AnimationState() != 2)
						playerControler[0]->StopDash();
				}
			}
		}

		//SwordAttack & PunchAttack & DistanceAttack
		if (playerControler[0]->AnimationState() != 8)	//죽지않을경우
		{
			//DistanceAttack
			{
				//move
				if (playerControler[0]->MoveMode() == false && player->CurrEquip() == 2)
				{
					if (Keyboard::Get()->Press('X') && playerControler[0]->AnimationState() != 9
						&& AttackState[0] != PlayerAttackState::RangeAttack)	//활 시위당기기
					{
						if (shootTime == false && (playerControler[0]->ArrowState() == 1 || playerControler[0]->ArrowState() == 2))
						{
							shootMotion = false;
							noAttack[0] = 1;
							playerControler[0]->InputArrowState(2);
							Vector3 tempMousePos = terrain->GetPickedPosition();
							if (tempMousePos.z < 256.0f && tempMousePos.z > -256.0f && tempMousePos.x < 256.0f && tempMousePos.x > -256.0f)
								playerControler[0]->Attack(4, player->GetTransform(0), terrain->GetPickedPosition(), Keyboard::Get()->Press('X'));
						}
					}
					if (Keyboard::Get()->Up('X') && playerControler[0]->AnimationState() != 9
						&& AttackState[0] != PlayerAttackState::RangeAttack)	//활 쏘기
					{
						if (playerControler[0]->ArrowState() == 2 && dontShootTime >= 0.3f)
						{
							noAttack[0] = 2;
							Vector3 tempMousePos = terrain->GetPickedPosition();
							if (tempMousePos.z < 256.0f && tempMousePos.z > -256.0f && tempMousePos.x < 256.0f && tempMousePos.x > -256.0f)
								playerControler[0]->Attack(4, player->GetTransform(0), terrain->GetPickedPosition(), Keyboard::Get()->Up('X'));
						}
					}
				}
				//move2
				else if (playerControler[0]->MoveMode() == true && player->CurrEquip() == 2)
				{
					if (Mouse::Get()->Press(0) && playerControler[0]->AnimationState() != 9
						&& AttackState[0] != PlayerAttackState::RangeAttack)	//활 시위당기기
					{
						if (shootTime == false && (playerControler[0]->ArrowState() == 1 || playerControler[0]->ArrowState() == 2))
						{
							shootMotion = false;
							noAttack[0] = 1;
							playerControler[0]->InputArrowState(2);
							playerControler[0]->Attack(4, player->GetTransform(0), terrain->GetPickedPosition(), Mouse::Get()->Press(0));
						}
					}
					if (Mouse::Get()->Up(0) && playerControler[0]->AnimationState() != 9
						&& AttackState[0] != PlayerAttackState::RangeAttack)	//활 쏘기
					{
						if (playerControler[0]->ArrowState() == 2 && dontShootTime >= 0.3f)
						{
							noAttack[0] = 2;
							playerControler[0]->Attack(4, player->GetTransform(0), terrain->GetPickedPosition(), Mouse::Get()->Up(0));
						}
					}
				}
				//시위를 놨을때
				if (playerControler[0]->AnimationState() != 9 || AttackState[0] != PlayerAttackState::RangeAttack)
				{
					//조준을 하고 0.3초내로 놨을때
					if (playerControler[0]->MoveMode() == false)
					{
						if (Keyboard::Get()->Press('X'))
							dontShootTime += Time::Get()->Delta();
						if (Keyboard::Get()->Up('X') && dontShootTime < 0.3f)
						{
							dontShootTime = 0.0f;
							playerControler[0]->InputArrowState(1);
							noAttack[0] = 0;
						}
					}
					else if (playerControler[0]->MoveMode() == true)
					{
						if (Mouse::Get()->Press(0))
							dontShootTime += Time::Get()->Delta();
						if (Mouse::Get()->Up(0) && dontShootTime < 0.3f)
						{
							dontShootTime = 0.0f;
							playerControler[0]->InputArrowState(1);
							noAttack[0] = 0;
						}
					}

					//화살의 피격 판정
					if (playerControler[0]->ArrowState() == 4 || playerControler[0]->ArrowState() == 5)
					{
						noAttack[0] = 0;
						playerTime[0] = 0;
						playerControler[0]->InputArrowState(1);
						AttackState[0] = PlayerAttackState::Idle;

						player->PlayClip(0, 0, 1.0f, 0.0f);
						playerControler[0]->StopDash();

						if (dontShootTime >= 0.3f && shootTime == true)
						{
							dontShootTime = 0.0f;
							shootTime = false;
						}
					}

					if (noAttack[0] == 2 && (playerControler[0]->ArrowState() == 2 || playerControler[0]->ArrowState() == 3))
					{
						playerControler[0]->DistanceAttacked(playerArrows[0]->GetTransform(0), terrain->GetPickedPosition());
						playerArrows[0]->UpdateTransforms();
						shootTime = true;

						if (playerTime[0] > 0.0f)
						{
							PlayerArrowUpdates();

							for (UINT i = 0; i < ARROWCOUNT; i++)
							{
								for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
									playerToMonster[i] = false;

								playerArrowColliders[i]->Collider->Update();
								playerArrows[i]->UpdateTransforms();
							}
						}
					}//if (noAttack[0] == 2)end
				}
			}
			

			if (Keyboard::Get()->Down('X') && playerControler[0]->AnimationState() != 9
				&& AttackState[0] != PlayerAttackState::RangeAttack)
			{
				if (player->CurrEquip() == 0)
				{
					noAttack[0]++;
					noAttack[0] %= 3;
					playerControler[0]->Attack(2, player->GetTransform(0), terrain->GetPickedPosition(), Keyboard::Get()->Down('X'));
				}
				if (player->CurrEquip() == 1)
				{
					noAttack[0]++;
					noAttack[0] %= 3;
					playerControler[0]->Attack(1, player->GetTransform(0), terrain->GetPickedPosition(), Keyboard::Get()->Down('X'));
				}
			}
			//RangeAttack
			else if (Keyboard::Get()->Down('C') && playerControler[0]->AnimationState() != 9
				&& rangeSkillOn == false)
			{
				if (player->CurrEquip() == 1 || player->CurrEquip() == 2)
				{
					if (AttackState[0] == PlayerAttackState::Idle)
					{
						for (int i = 0; i < PARTICLECOUNT; i++)
						{
							explosion[i]->Start();
							explosion[i]->Reset();
						}
						rangeSkillPos = terrain->GetPickedPosition();
						AttackState[0] = PlayerAttackState::RangeAttack;
						rangeSkillOn = true;

						Vector3 poss;
						Context::Get()->GetCamera()->Position(&poss);
						rangeCameraPos = poss;
					}
				}
			}

			//Dash
			if (Keyboard::Get()->Down(VK_SPACE) && playerControler[0]->AnimationState() != 9)
			{
				Vector3 tempMousePos = terrain->GetPickedPosition();
				if (tempMousePos.z < 256.0f && tempMousePos.z > -256.0f &&
					tempMousePos.x < 256.0f && tempMousePos.x > -256.0f)
						mousePos = terrain->GetPickedPosition();
			}
		}//if (playerControler[0]->AnimationState() != 8)end

		playerHp->HpTransform(player->GetTransform(0));	// 플레이어 체력

		//player
		if (playerControler[0]->AnimationState() != 8)
		{
			if (AttackState[0] != PlayerAttackState::RangeAttack)
			{
				//dash skill
				playerControler[0]->Dash(player->GetTransform(0), mousePos, Keyboard::Get()->Down(VK_SPACE));
				//player move
				if (playerControler[0]->AnimationState() == 2 || playerControler[0]->AnimationState() == 10
					|| playerControler[0]->AnimationState() == 6)	//move, run, distanceMove
				{
					if (playerControler[0]->MoveMode() == false)
						playerControler[0]->Move(player->GetTransform(0), mousePos, false, player->CurrEquip(), terrain->GetHeight(pos));
					else if (playerControler[0]->MoveMode() == true)
						playerControler[0]->Move2(player->GetTransform(0), player->CurrEquip(), terrain->GetHeight(pos));
				}

			}
			//range skill
			if (rangeSkillOn == true)
			{
				RangeAttacked(rangeSkillPos);
			}

			switch (playerControler[0]->AnimationState())
			{
			case 1:	//idle
				if (AttackState[0] == PlayerAttackState::SwordAttack ||
					AttackState[0] == PlayerAttackState::PunchAttack ||
					AttackState[0] == PlayerAttackState::DistanceAttack)
					AttackState[0] = PlayerAttackState::Idle;
				player->PlayClip(0, 0, 1.0f, 0.5f);

				for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
					playerToMonster[i] = false;
				break;
			case 2:	//move
				if (AttackState[0] == PlayerAttackState::SwordAttack ||
					AttackState[0] == PlayerAttackState::PunchAttack ||
					AttackState[0] == PlayerAttackState::DistanceAttack)
					AttackState[0] = PlayerAttackState::Idle;
				//if (playerControler[0]->MoveMode() == false)
				//	noAttack[0] = 0;

				if (player->CurrEquip() == 1)
					player->PlayClip(0, 1, 1.0f, 0.0f);
				else if (player->CurrEquip() == 2)
					player->PlayClip(0, 1, 1.5f, 0.0f);

				for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
					playerToMonster[i] = false;
				break;
			case 3:	//sword attack
				PlayerAttackControl(0);
				break;
			case 4:	//punch attack
				PlayerAttackControl(0);
				break;
			case 5:	//range attack
				if (AttackState[0] != PlayerAttackState::Idle)
					player->PlayClip(0, 2, 1.0f, 0.0f);

				if (player->StopClip(0, 2))
				{
					for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
						playerToMonster[i] = false;
					AttackState[0] = PlayerAttackState::Idle;
					player->PlayClip(0, 0, 1.0f, 0.5f);
				}
				break;
			case 6:	//distance attack
				PlayerAttackControl(0);
				break;
			case 9:	//dash
				if (AttackState[0] == PlayerAttackState::SwordAttack ||
					AttackState[0] == PlayerAttackState::PunchAttack ||
					AttackState[0] == PlayerAttackState::DistanceAttack)
						AttackState[0] = PlayerAttackState::Idle;

				player->PlayClip(0, 6, 2.0f, 0.1f);

				for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
					playerToMonster[i] = false;

				if (player->StopClip(0, 6) == true)
				{
					player->PlayClip(0, 0, 1.0f, 0.0f);
					playerControler[0]->StopDash();
				}
				break;
			case 10: //run
				if (AttackState[0] == PlayerAttackState::SwordAttack ||
					AttackState[0] == PlayerAttackState::PunchAttack ||
					AttackState[0] == PlayerAttackState::DistanceAttack)
					AttackState[0] = PlayerAttackState::Idle;
				//noAttack[0] = 0;
				player->PlayClip(0, 9, 1.0f, 0.0f);

				for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
					playerToMonster[i] = false;
				break;
			}
			player->UpdateTransforms();
		}

		//몬스터체력
		for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
		{
			if (i < MONSTERCOUNT)
				monsterHp[i]->HpTransform(monster->GetTransform(i));
			else
				monsterHp[i]->HpTransform(rangeMonster->GetTransform(i - MONSTERCOUNT));
		}

		//monster
		{
			for (int i = 0; i < MONSTERCOUNT; i++)
			{
				monsterControler[i]->Patrol(monster->GetTransform(i), pos, false, terrain->GetHeight(pos));

				switch (monsterControler[i]->AnimationState())
				{
				case 1:
					monster->PlayClip(i, 0, 1.0f, 0.3f);
					break;
				case 2:
					if (monsterControler[i]->autoAttackState() != 4)
						monster->PlayClip(i, 1, 1.0f, 0.0f);
					else if (monsterControler[i]->autoAttackState() == 4)
						monster->PlayClip(i, 1, 2.0f, 0.0f);
					break;
				case 4:
					monster->PlayClip(i, 2, 1.2f, 0.3f);

					if (monster->StopClip(i, 2) == true)	//때린후에 움직일수있게
					{
						monsterControler[i]->Attack(2, monster->GetTransform(i), pos, true);
						monsterToPlayer[i] = false;
						monsterControler[i]->MoveStart();
					}
					break;
				}
				monster->UpdateTransforms();
			}
		}


		//rangeMonster
		{
			for (int i = 0; i < RANGEMONSTERCOUNT; i++)
			{
				rangeMonsterControler[i]->Patrol(rangeMonster->GetTransform(i), pos, true, terrain->GetHeight(pos));

				switch (rangeMonsterControler[i]->AnimationState())
				{
				case 1:
					rangeMonster->PlayClip(i, 0, 1.0f, 0.3f);
					break;
				case 2:
					if (rangeMonsterControler[i]->autoAttackState() != 4)
						rangeMonster->PlayClip(i, 1, 1.0f, 0.0f);
					else if (rangeMonsterControler[i]->autoAttackState() == 4)
						rangeMonster->PlayClip(i, 1, 2.0f, 0.0f);
					break;
				case 6:	//distance attack
					if (rangeMonsterTime[i] == 0 && monsterNoAttack[i] == 0)
					{
						monsterNoAttack[i] = 1;
						rangeMonsterControler[i]->Attack(4, rangeMonster->GetTransform(i), pos, true);
						rangeMonsterControler[i]->InputArrowState(2);
						rangeMonsterControler[i]->ArriveArrowShoot(pos);
					}
					if (rangeMonsterTime[i] >= 2.0f && monsterNoAttack[i] == 1)
					{
						monsterNoAttack[i] = 2;
					}

					MonsterAttackControl(i, pos);

					if (rangeMonster->StopClip(i, 5) == true)	//때린후에 움직일수있게
					{
						monsterToPlayer[RANGEMONSTERCOUNT + i] = false;
						rangeMonsterControler[i]->MoveStart();
					}
					break;
				}
				rangeMonster->UpdateTransforms();
			}
		}
	}


	//플레이어 충돌체 update
	for (UINT i = 0; i < PLAYERCOUNT; i++)
	{
		Transform* transform = player->GetTransform(i);
		Vector3 pos;
		Vector3 rot;
		Vector3 scale;
		transform->Position(&pos);
		transform->RotationDegree(&rot);
		transform->Scale(&scale);
		playerColliders[i]->Collider->GetTransform()->Position(pos);
		playerColliders[i]->Collider->GetTransform()->RotationDegree(rot);
		playerColliders[i]->Collider->GetTransform()->Scale(scale);
		playerColliders[i]->Collider->Update();
	}

	//monster collider update
	for (UINT i = 0; i < MONSTERCOUNT; i++)
	{
		Transform* transform = monster->GetTransform(i);
		Vector3 pos;
		Vector3 rot;
		Vector3 scale;
		transform->Position(&pos);
		transform->RotationDegree(&rot);
		transform->Scale(&scale);
		monsterColliders[i]->Collider->GetTransform()->Position(pos);
		monsterColliders[i]->Collider->GetTransform()->RotationDegree(rot);
		monsterColliders[i]->Collider->GetTransform()->Scale(scale);
		monsterColliders[i]->Collider->Update();

		//weapon collider
		{
			Matrix attach = monster->GetAttachTransform(i, 20);
			monsterAttackColliders[i]->Collider->GetTransform()->World(attach);
			monsterAttackColliders[i]->Collider->Update();
		}
	}

	//(rangeMonster && weapon) collider update
	for (UINT i = 0; i < RANGEMONSTERCOUNT; i++)
	{
		Transform* transform = rangeMonster->GetTransform(i);
		Vector3 pos;
		Vector3 rot;
		Vector3 scale;
		transform->Position(&pos);
		transform->RotationDegree(&rot);
		transform->Scale(&scale);
		rangeMonsterColliders[i]->Collider->GetTransform()->Position(pos);
		rangeMonsterColliders[i]->Collider->GetTransform()->RotationDegree(rot);
		rangeMonsterColliders[i]->Collider->GetTransform()->Scale(scale);
		rangeMonsterColliders[i]->Collider->Update();


		//weapon collider
		{
			Matrix attach = rangeMonster->GetAttachTransform(i, 20);
			monsterAttackColliders[MONSTERCOUNT + i]->Collider->GetTransform()->World(attach);
			monsterAttackColliders[MONSTERCOUNT + i]->Collider->Update();
		}
	}

	//무기 update
	for (UINT i = 0; i < WEAPONCOUNT; i++)
	{
		if (i < 1)
		{
			Matrix attach = player->GetAttachTransform(0, 35);
			weaponColliders[i]->Collider->GetTransform()->World(attach);
			weaponColliders[i]->Collider->Update();
		}
		else
		{
			weaponColliders[i]->Collider->GetTransform()->Position(rangeSkillPos);
			weaponColliders[i]->Collider->Update();
		}
	}

	//떨어진 무기 update
	if (getWeaponed[0] == false || getWeaponed[1] == false)
	{
		for (UINT i = 0; i < WEAPONCOUNT; i++)
		{
			Transform* transform = dropWeapons[i]->GetTransform(0);
			Vector3 pos;
			Vector3 scale;
			transform->Position(&pos);
			transform->Scale(&scale);

			dropWeaponColliders[i]->Collider->GetTransform()->Position(pos);
			dropWeaponColliders[i]->Collider->GetTransform()->Scale(scale);

			if (dropWeaponColliders[i]->Collider->IsIntersect(playerColliders[0]->Collider))
			{
				switch (i)
				{
				case 0:
					if (getWeaponed[0] == false)
					{
						if (player->CurrEquip() == 0)
						{
							getWeaponed[0] = true;
							Lights::Get()->GetSpotLight(0).Range = 0;
						}
						if (player->CurrEquip() == 2)
						{
							getWeaponed[0] = true;
							Lights::Get()->GetSpotLight(0).Range = 0;
						}
					}
					break;
				case 1:
					if (getWeaponed[1] == false)
					{
						if (player->CurrEquip() == 0)
						{
							getWeaponed[1] = true;
							Lights::Get()->GetSpotLight(1).Range = 0;
						}
						if (player->CurrEquip() == 1)
						{
							getWeaponed[1] = true;
							Lights::Get()->GetSpotLight(1).Range = 0;
						}
					}
					break;
				}
			}
			dropWeaponColliders[i]->Collider->Update();
		}

		dropWeapons[0]->Update();
		dropWeapons[1]->Update();
	}

	//monster피격 update
	for (UINT i = 0; i < MONSTERCOUNT; i++)
	{
		if (weaponColliders[0]->Collider->IsIntersect(monsterColliders[i]->Collider) &&
			AttackState[0] == PlayerAttackState::PunchAttack)
		{
			if (playerToMonster[i] == false)
				monsterHp[i]->Dameged(5.0f);

			if (playerToMonster[i] == false)
				playerToMonster[i] = true;
		}
		if (weaponColliders[0]->Collider->IsIntersect(monsterColliders[i]->Collider) &&
			AttackState[0] == PlayerAttackState::SwordAttack)
		{
			if (playerToMonster[i] == false)
				monsterHp[i]->Dameged(20.0f);

			if (playerToMonster[i] == false)
				playerToMonster[i] = true;
		}
		if (weaponColliders[1]->Collider->IsIntersect(monsterColliders[i]->Collider) &&
			AttackState[0] == PlayerAttackState::RangeAttack && rangeSkillCount < 4)	//터지기가 생성될때까지만 적용
		{
			if (playerToMonster[i] == false)
				monsterHp[i]->Dameged(50.0f);

			if (playerToMonster[i] == false)
				playerToMonster[i] = true;
		}
		if (playerArrowColliders[0]->Collider->IsIntersect(monsterColliders[i]->Collider) &&
			playerControler[0]->ArrowState() == 3)
		{
			if (playerToMonster[i] == false)
				monsterHp[i]->Dameged(10.0f);

			if (playerToMonster[i] == false)
				playerToMonster[i] = true;

			//화살에 첫번째로 닿은 적만 죽음
			{
				PlayerArrowUpdates();

				for (UINT j = 0; j < ARROWCOUNT; j++)
				{
					playerArrowColliders[j]->Collider->Update();
					playerArrows[j]->UpdateTransforms();
				}

				noAttack[0] = 0;
				playerTime[0] = 0;
				AttackState[0] = PlayerAttackState::Idle;
				playerControler[0]->InputArrowState(4);
			}
		}

		if (monsterHp[i]->HpState() <= 0)
		{
			monsterControler[i]->DeathUnit();
			monster->PlayClip(i, 3, 1.2f, 0.3f);
		}

		if (monsterControler[i]->AnimationState() == 8)
		{
			if (monster->StopClip(i, 3))
			{
				monster->GetTransform(i)->Position(-5, 0, -5);
				monster->GetTransform(i)->Scale(0, 0, 0);
				monster->UpdateTransforms();
			}
		}
	}

	//rangeMonster 피격 update
	for (UINT i = 0; i < RANGEMONSTERCOUNT; i++)
	{
		if (weaponColliders[0]->Collider->IsIntersect(rangeMonsterColliders[i]->Collider) &&
			AttackState[0] == PlayerAttackState::PunchAttack)
		{
			if (playerToMonster[MONSTERCOUNT + i] == false)
				monsterHp[MONSTERCOUNT + i]->Dameged(5.0f);

			if (playerToMonster[MONSTERCOUNT + i] == false)
				playerToMonster[MONSTERCOUNT + i] = true;
		}
		if (weaponColliders[0]->Collider->IsIntersect(rangeMonsterColliders[i]->Collider) &&
			AttackState[0] == PlayerAttackState::SwordAttack)
		{
			if (playerToMonster[MONSTERCOUNT + i] == false)
				monsterHp[MONSTERCOUNT + i]->Dameged(20.0f);

			if (playerToMonster[MONSTERCOUNT + i] == false)
				playerToMonster[MONSTERCOUNT + i] = true;
		}
		if (weaponColliders[1]->Collider->IsIntersect(rangeMonsterColliders[i]->Collider) &&
			AttackState[0] == PlayerAttackState::RangeAttack && rangeSkillCount < 4)	//터지기가 생성될때까지만 적용
		{
			if (playerToMonster[MONSTERCOUNT + i] == false)
				monsterHp[MONSTERCOUNT + i]->Dameged(50.0f);

			if (playerToMonster[MONSTERCOUNT + i] == false)
				playerToMonster[MONSTERCOUNT + i] = true;
		}
		if (playerArrowColliders[0]->Collider->IsIntersect(rangeMonsterColliders[i]->Collider) &&
			playerControler[0]->ArrowState() == 3)
		{
			if (playerToMonster[MONSTERCOUNT + i] == false)
				monsterHp[MONSTERCOUNT + i]->Dameged(10.0f);

			if (playerToMonster[MONSTERCOUNT + i] == false)
				playerToMonster[MONSTERCOUNT + i] = true;

			//화살에 첫번째로 닿은 적만 죽음
			{
				PlayerArrowUpdates();

				for (UINT j = 0; j < ARROWCOUNT; j++)
				{
					playerArrowColliders[j]->Collider->Update();
					playerArrows[j]->UpdateTransforms();
				}

				noAttack[0] = 0;
				playerTime[0] = 0;
				AttackState[0] = PlayerAttackState::Idle;
				playerControler[0]->InputArrowState(4);
			}
		}

		if (monsterHp[MONSTERCOUNT + i]->HpState() <= 0)
		{
			rangeMonsterControler[i]->DeathUnit();
			rangeMonster->PlayClip(i, 3, 1.2f, 0.3f);
		}

		if (rangeMonsterControler[i]->AnimationState() == 8)
		{
			if (rangeMonster->StopClip(i, 3))
			{
				rangeMonster->GetTransform(i)->Position(-5, 0, -5);
				rangeMonster->GetTransform(i)->Scale(0, 0, 0);
				rangeMonster->UpdateTransforms();
			}
		}
	}

	//player피격
	for (UINT i = 0; i < PLAYERCOUNT; i++)
	{
		for (UINT j = 0; j < MONSTERCOUNT; j++)
		{
			if (monsterAttackColliders[j]->Collider->IsIntersect(playerColliders[i]->Collider) &&
				monsterControler[j]->AnimationState() == 4 && playerControler[i]->AnimationState() != 9)
			{
				if (monsterToPlayer[j] == false)
					playerHp->Dameged(10.0f);

				if (monsterToPlayer[j] == false)
					monsterToPlayer[j] = true;
			}
		}//monster for
		for (UINT j = 0; j < RANGEMONSTERCOUNT; j++)
		{
			if (monsterArrowColliders[j]->Collider->IsIntersect(playerColliders[i]->Collider) &&
				rangeMonsterControler[j]->AnimationState() == 6 && playerControler[i]->AnimationState() != 9)
			{
				if (monsterToPlayer[RANGEMONSTERCOUNT + j] == false)
					playerHp->Dameged(5.0f);

				if (monsterToPlayer[RANGEMONSTERCOUNT + j] == false)
					monsterToPlayer[RANGEMONSTERCOUNT + j] = true;
			}
		}

		if (playerHp->HpState() <= 0)
		{
			playerControler[i]->DeathUnit();
			player->PlayClip(i, 5, 1.2f, 0.3f);
		}

		if (playerControler[i]->AnimationState() == 8)
		{
			if (player->StopClip(i, 5))
			{
				playerDeath = true;
			}
		}
	}

	//ImGui::ShowDemoWindow();
	//ImGui::BeginPopupContextWindow();

	//화살
	{
		PlayerArrowUpdates();
		for (UINT i = 0; i < ARROWCOUNT; i++)
		{
			playerArrowColliders[i]->Collider->Update();
			playerArrows[i]->UpdateTransforms();
			playerArrows[i]->Update();
		}
		for (UINT i = 0; i < RANGEMONSTERCOUNT; i++)
		{
			MonsterArrowUpdates(i);
			monsterArrowColliders[i]->Collider->Update();
			monsterArrows[i]->UpdateTransforms();
			monsterArrows[i]->Update();
		}
	}

	skyDome->Update();
	water->Update();

	for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
		monsterHp[i]->Update();

	if (playerDeath == false)
	{
		playerHp->Update();
		player->Update();
	}

	monster->Update();
	rangeMonster->Update();

	terrain->Update();
}

void Battle::PreRender()
{
	skyDome->PreRender();

	//Depth
	{
		shadow->PreRender();
		terrain->Pass(3);
		terrain->Render();

		Pass(0, 1, 2);

		player->Render();
		monster->Render();
		rangeMonster->Render();
	}
	//Reflection
	{
		water->PreRender_Reflection();

		skyDome->Pass(10, 11, 12);
		skyDome->Render();

		Pass(13, 14, 15);

		player->Render();
		monster->Render();
		rangeMonster->Render();
	}

	//Refraction
	{
		water->PreRender_Refraction();

		//skyDome->Pass(4, 5, 6);
		//skyDome->Render();

		Pass(7, 8, 9);

		player->Render();
		monster->Render();
		rangeMonster->Render();
	}


	if (playerControler[0]->AnimationState() == 6 &&
		(noAttack[0] == 2 || noAttack[0] == 1))
	{
		for (UINT i = 0; i < ARROWCOUNT; i++)
		{
			playerArrows[i]->Render();
		}
	}

	for (int i = 0; i < RANGEMONSTERCOUNT; i++)
	{
		if (rangeMonsterControler[i]->AnimationState() == 6 &&
			(monsterNoAttack[i] == 2 || monsterNoAttack[i] == 1))
		{
			monsterArrows[i]->Render();
		}
	}

	if (rangeSkillDelay <= 3.0f)
	{
		for (int i = 0; i < PARTICLECOUNT; i++)
		{
			explosion[i]->Render();
		}
	}
}

void Battle::Render()
{
	skyDome->Pass(4, 5, 6);
	skyDome->Render();

	Pass(7, 8, 9);

	terrain->Pass(0);
	terrain->Render();

	player->Render();
	monster->Render();
	rangeMonster->Render();

	if (noAttack[0] == 2 || playerControler[0]->ArrowState() == 2 ||
		playerControler[0]->ArrowState() == 3)
	{
		for (UINT i = 0; i < ARROWCOUNT; i++)
		{
			playerArrows[i]->Render();
		}
	}

	for (int i = 0; i < RANGEMONSTERCOUNT; i++)
	{
		if (rangeMonsterControler[i]->AnimationState() == 6 &&
			(monsterNoAttack[i] == 2 || monsterNoAttack[i] == 1))
		{
			monsterArrows[i]->Render();
		}
	}

	if (getWeaponed[0] == false)
		dropWeapons[0]->Render();
	if (getWeaponed[1] == false)
		dropWeapons[1]->Render();

	//for (int i = 0; i < PLAYERCOUNT; i++)
	//	playerColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	//for (int i = 0; i < WEAPONCOUNT; i++)
	//	weaponColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	//for (int i = 0; i < MONSTERCOUNT; i++)
	//	monsterColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	//for (int i = 0; i < DROPWEAPONCOUNT; i++)
	//	dropWeaponColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	//for (int i = 0; i < ARROWCOUNT; i++)
	//	playerArrowColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	//for (int i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
	//	monsterAttackColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	//for (int i = 0; i < RANGEMONSTERCOUNT; i++)
	//	monsterArrowColliders[i]->Collider->Render(Color(0, 1, 0, 1));

	isRender = true;

	if (rangeSkillDelay <= 3.0f)
	{
		for (int i = 0; i < PARTICLECOUNT; i++)
		{
			explosion[i]->Render();
		}
	}

	//water->Pass(16);
	//water->Render();

	if (playerControler[0]->AnimationState() != 8)
		playerHp->Render();

	for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
	{
		if (monsterControler[i]->AnimationState() != 8 && i < MONSTERCOUNT)
		{
			monsterHp[i]->Render();
		}
		else if (rangeMonsterControler[i - MONSTERCOUNT]->AnimationState() != 8 && MONSTERCOUNT <= i)
		{
			monsterHp[i]->Render();
		}
	}
}

//void Battle::Mesh()
//{
//	//Create Material
//	{
//		floor = new Material(shader);
//		floor->DiffuseMap("Floor.png");
//		floor->Specular(1, 1, 1, 15);
//		floor->SpecularMap("Floor_Specular.png");
//		floor->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
//		floor->NormalMap("Floor_Normal.png");
//	}
//
//	//Create Mesh
//	{
//		Transform* transform = NULL;
//
//		grid = new MeshRender(shader, new MeshGrid(5, 5));
//		transform = grid->AddTransform();
//		transform->Position(0, 0, 0);
//		transform->Scale(1, 1, 1);
//	}
//
//	grid->UpdateTransforms();
//
//	meshes.push_back(grid);
//}

void Battle::Player()
{
	for (int i = 0; i < 2; i++)
	{
		weapon[i] = new Model();
	}

	weapon[0]->ReadMaterial(L"Weapon/Sword");
	weapon[0]->ReadMesh(L"Weapon/Sword");
	weapon[1]->ReadMaterial(L"Weapon/LongBow");
	weapon[1]->ReadMesh(L"Weapon/LongBow");

	player = new ModelAnimator(shader);
	player->ReadMaterial(L"Player/Player");
	player->ReadMesh(L"Player/Player");

	player->ReadClip(L"Player/Idle");		//0
	player->ReadClip(L"Player/Move");	//1
	//Sword
	player->ReadClip(L"Player/Sword_Melee_Attack");		//2
	player->ReadClip(L"Player/Sword_Attack_360Rolling");	//3
	player->ReadClip(L"Player/Melee_Punch");		//4
	player->ReadClip(L"Player/Death");			//5
	player->ReadClip(L"Player/Forward Roll");	//6
	player->ReadClip(L"Player/Standing_Aim_Overdraw");	//7 활 조준
	player->ReadClip(L"Player/Standing_Aim_Recoil");	//8 화살 발사
	player->ReadClip(L"Player/Running");	//9
	//arrow shoot move
	player->ReadClip(L"Player/standing aim walk back");	//10
	player->ReadClip(L"Player/standing aim walk forward");	//11
	player->ReadClip(L"Player/standing aim walk left");	//12
	player->ReadClip(L"Player/standing aim walk right");	//13

	Transform attachTransform[2];

	//Sword
	attachTransform[0].Position(3.5f, 10, -3.5f);
	attachTransform[0].RotationDegree(0, 0, 90);
	attachTransform[0].Scale(1.28f, 1.28f, 1.28f);

	//Bow
	attachTransform[1].Position(0, -0.5f, 0);
	attachTransform[1].RotationDegree(0, 100, -175);
	attachTransform[1].Scale(0.18f, 0.18f, 0.18f);

	player->GetModel()->Attach(shader, weapon[0], 35, &attachTransform[0]);
	player->GetModel()->Attach(shader, weapon[1], 20, &attachTransform[1]);


	Transform* transform = NULL;

	transform = player->AddTransform();
	transform->Position(-25 + 80, 0, -30 + 80);
	transform->Scale(0.075f, 0.075f, 0.075f);
	player->PlayClip(0, 0, 1.0f);

	/*transform = player->AddTransform();
	transform->Position(40, 0, 40);
	transform->Scale(0.075f, 0.075f, 0.075f);
	player->PlayClip(1, 0, 1.0f);*/

	player->UpdateTransforms();

	animators.push_back(player);

	playerColliders = new ColliderObjectDesc*[PLAYERCOUNT];
	for (UINT i = 0; i < PLAYERCOUNT; i++)
	{
		playerColliders[i] = new ColliderObjectDesc();

		playerColliders[i]->Init = new Transform();
		playerColliders[i]->Init->Position(0, 100, 0);
		playerColliders[i]->Init->Scale(100, 200, 100);

		playerColliders[i]->Transform = new Transform();
		playerColliders[i]->Collider = new Collider(playerColliders[i]->Transform, playerColliders[i]->Init);

		playerControler[i] = new ModelControler();

		//MoveModeSetting
		playerControler[i]->InputMoveMode(battleStyle);
		playerControler[i]->InputArrowState(1);

		noAttack.push_back(dontAttack);
		playerTime.push_back(timing);
		AttackState.push_back(playerAttackState);
	}

	weaponColliders = new ColliderObjectDesc*[WEAPONCOUNT];
	for (UINT i = 0; i < WEAPONCOUNT; i++)
	{
		weaponColliders[i] = new ColliderObjectDesc();
		weaponColliders[i]->Init = new Transform();

		if (i == 0)
		{
			weaponColliders[i]->Init->Position(-10, -3.5f, -22.5f);
			weaponColliders[i]->Init->Scale(10, 10, 85);
		}
		else if (i == 1)
		{
			weaponColliders[i]->Init->Position(0, 10, 0);
			weaponColliders[i]->Init->Scale(20, 20, 20);
		}

		weaponColliders[i]->Transform = new Transform();
		weaponColliders[i]->Collider = new Collider(weaponColliders[i]->Transform, weaponColliders[i]->Init);
	}
}

void Battle::Monster()
{
	//monster
	monster = new ModelAnimator(shader);
	monster->ReadMaterial(L"Monster/Monster");
	monster->ReadMesh(L"Monster/Monster");
	monster->ReadClip(L"Monster/LookAround_Idle");
	monster->ReadClip(L"Monster/Monster Walk");
	monster->ReadClip(L"Monster/Monster Attack");
	monster->ReadClip(L"Monster/Monster Death");

	//rangemonster
	rangeMonster = new ModelAnimator(shader);
	rangeMonster->ReadMaterial(L"Monster/Monster");
	rangeMonster->ReadMesh(L"Monster/Monster");
	rangeMonster->ReadClip(L"Monster/LookAround_Idle");	//0
	rangeMonster->ReadClip(L"Monster/Monster Walk");	//1
	rangeMonster->ReadClip(L"Monster/Monster Attack");	//2
	rangeMonster->ReadClip(L"Monster/Monster Death");	//3
	rangeMonster->ReadClip(L"Player/Standing_Aim_Overdraw");	//4 활 조준
	rangeMonster->ReadClip(L"Player/Standing_Aim_Recoil");	//5 화살 발사

	//monsterRangeWeapon
	monsterRangeWeapon[0] = new Model();
	monsterRangeWeapon[1] = new Model();

	monsterRangeWeapon[0]->ReadMaterial(L"Weapon/LongBow");
	monsterRangeWeapon[0]->ReadMesh(L"Weapon/LongBow");

	monsterRangeWeapon[1]->ReadMaterial(L"Weapon/Sword");
	monsterRangeWeapon[1]->ReadMesh(L"Weapon/Sword");

	Transform attachTransform[2];
	//Bow
	attachTransform[0].Position(0, 2.0f, 0);
	attachTransform[0].RotationDegree(0, 90, -90);
	attachTransform[0].Scale(0.18f, 0.18f, 0.18f);

	//attack attach
	attachTransform[1].Position(3.5f, 10, -3.5f);
	attachTransform[1].RotationDegree(0, 0, 90);
	attachTransform[1].Scale(1.28f, 1.28f, 1.28f);

	monster->GetModel()->Attach(shader, monsterRangeWeapon[1], 20, &attachTransform[1]);	//타격용
	monster->Equipment(0);

	rangeMonster->GetModel()->Attach(shader, monsterRangeWeapon[1], 20, &attachTransform[1]);	//타격용
	rangeMonster->GetModel()->Attach(shader, monsterRangeWeapon[0], 35, &attachTransform[0]);
	rangeMonster->Equipment(2);

	Transform* transform = NULL;
	//monster
	transform = monster->AddTransform();
	transform->Position(80, 0, 80);
	transform->Scale(0.075f, 0.075f, 0.075f);
	monster->PlayClip(0, 0, 1.0f);

	transform = monster->AddTransform();
	transform->Position(100, 0, 100);
	transform->Scale(0.075f, 0.075f, 0.075f);
	monster->PlayClip(1, 0, 1.0f);

	transform = monster->AddTransform();
	transform->Position(60, 0, 90);
	transform->Scale(0.075f, 0.075f, 0.075f);
	monster->PlayClip(2, 0, 1.0f);

	transform = monster->AddTransform();
	transform->Position(30, 0, 60);
	transform->Scale(0.075f, 0.075f, 0.075f);
	monster->PlayClip(3, 0, 1.0f);

	transform = monster->AddTransform();
	transform->Position(120, 0, 30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	monster->PlayClip(4, 0, 1.0f);

	transform = monster->AddTransform();
	transform->Position(120, 0, 70);
	transform->Scale(0.075f, 0.075f, 0.075f);
	monster->PlayClip(5, 0, 1.0f);


	//rangemonster
	transform = rangeMonster->AddTransform();
	transform->Position(110, 0, 50);
	transform->Scale(0.075f, 0.075f, 0.075f);
	rangeMonster->PlayClip(0, 0, 1.0f);

	transform = rangeMonster->AddTransform();
	transform->Position(110, 0, 40);
	transform->Scale(0.075f, 0.075f, 0.075f);
	rangeMonster->PlayClip(0, 0, 1.0f);

	monster->UpdateTransforms();
	rangeMonster->UpdateTransforms();

	animators.push_back(monster);
	animators.push_back(rangeMonster);

	//monster
	monsterColliders = new ColliderObjectDesc*[MONSTERCOUNT];
	for (UINT i = 0; i < MONSTERCOUNT; i++)
	{
		monsterTime.push_back(0);

		monsterControler[i] = new ModelControler();
		monsterColliders[i] = new ColliderObjectDesc();

		monsterColliders[i]->Init = new Transform();
		monsterColliders[i]->Init->Position(0, 100, 0);
		monsterColliders[i]->Init->Scale(100, 200, 100);

		monsterColliders[i]->Transform = new Transform();
		monsterColliders[i]->Collider = new Collider(monsterColliders[i]->Transform, monsterColliders[i]->Init);
	}

	//rangemonster
	rangeMonsterColliders = new ColliderObjectDesc*[RANGEMONSTERCOUNT];
	for (UINT i = 0; i < RANGEMONSTERCOUNT; i++)
	{
		monsterTime.push_back(0);
		rangeMonsterTime.push_back(0);
		monsterNoAttack.push_back(dontAttack);
		MonsterAttackState.push_back(playerAttackState);

		rangeMonsterControler[i] = new ModelControler();
		rangeMonsterColliders[i] = new ColliderObjectDesc();

		rangeMonsterColliders[i]->Init = new Transform();
		rangeMonsterColliders[i]->Init->Position(0, 100, 0);
		rangeMonsterColliders[i]->Init->Scale(100, 200, 100);

		rangeMonsterColliders[i]->Transform = new Transform();
		rangeMonsterColliders[i]->Collider = new Collider(rangeMonsterColliders[i]->Transform, rangeMonsterColliders[i]->Init);
	}


	//monsterAttackColliders
	monsterAttackColliders = new ColliderObjectDesc*[MONSTERCOUNT + RANGEMONSTERCOUNT];
	for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
	{
		monsterAttackColliders[i] = new ColliderObjectDesc();
		monsterAttackColliders[i]->Init = new Transform();

		monsterAttackColliders[i]->Init->Position(0, 0, 0);
		monsterAttackColliders[i]->Init->Scale(10, 10, 10);

		monsterAttackColliders[i]->Transform = new Transform();
		monsterAttackColliders[i]->Collider = new Collider(monsterAttackColliders[i]->Transform, monsterAttackColliders[i]->Init);
	}
}

void Battle::NoAttachWeapons()
{
	//땅에 있는 무기생성
	{
		dropWeapons[0] = new ModelRender(shader);
		dropWeapons[1] = new ModelRender(shader);

		dropWeapons[0]->ReadMaterial(L"Weapon/Sword");
		dropWeapons[0]->ReadMesh(L"Weapon/Sword");
		dropWeapons[1]->ReadMaterial(L"Weapon/LongBow");
		dropWeapons[1]->ReadMesh(L"Weapon/LongBow");

		Transform* transform = NULL;

		transform = dropWeapons[0]->AddTransform();
		transform->Position(60, 1, 30);
		transform->Scale(0.06f, 0.06f, 0.06f);

		transform = dropWeapons[1]->AddTransform();
		transform->Position(100, 1, 30);
		transform->Scale(0.4f, 0.4f, 0.4f);

		dropWeapons[0]->UpdateTransforms();
		dropWeapons[1]->UpdateTransforms();

		models.push_back(dropWeapons[0]);
		models.push_back(dropWeapons[1]);

		dropWeaponColliders = new ColliderObjectDesc*[WEAPONCOUNT];
		for (UINT i = 0; i < WEAPONCOUNT; i++)
		{
			dropWeaponColliders[i] = new ColliderObjectDesc();

			dropWeaponColliders[i]->Init = new Transform();

			if (i == 0)
			{
				dropWeaponColliders[i]->Init->Position(0, 25, 0);
				dropWeaponColliders[i]->Init->Scale(100, 50, 200);
			}
			if (i == 1)
			{
				dropWeaponColliders[i]->Init->Position(0, 5, 0);
				dropWeaponColliders[i]->Init->Scale(30, 10, 10);
			}

			dropWeaponColliders[i]->Transform = new Transform();
			dropWeaponColliders[i]->Collider = new Collider(dropWeaponColliders[i]->Transform, dropWeaponColliders[i]->Init);
		}
	}

	//화살 생성
	{
		for (UINT i = 0; i < ARROWCOUNT; i++)
		{
			playerArrows[i] = new ModelRender(shader);
			playerArrows[i]->ReadMaterial(L"Weapon/LongArrow");
			playerArrows[i]->ReadMesh(L"Weapon/LongArrow");

			Transform* transform = NULL;

			transform = playerArrows[i]->AddTransform();
			transform->Position(1, 1, 1);
			transform->RotationDegree(0, 0, 0);
			transform->Scale(1.2f, 0.4f, 1.2f);

			playerArrows[i]->UpdateTransforms();

			models.push_back(playerArrows[i]);
		}

		playerArrowColliders = new ColliderObjectDesc*[ARROWCOUNT];
		for (UINT i = 0; i < ARROWCOUNT; i++)
		{
			playerArrowColliders[i] = new ColliderObjectDesc();

			playerArrowColliders[i]->Init = new Transform();
			playerArrowColliders[i]->Init->Position(0, 0, 0);
			playerArrowColliders[i]->Init->RotationDegree(0, 0, 0);
			playerArrowColliders[i]->Init->Scale(1, 1, 1);

			playerArrowColliders[i]->Transform = new Transform();
			playerArrowColliders[i]->Collider = new Collider(playerArrowColliders[i]->Transform, playerArrowColliders[i]->Init);
		}

		for (UINT i = 0; i < RANGEMONSTERCOUNT; i++)
		{
			monsterArrows[i] = new ModelRender(shader);
			monsterArrows[i]->ReadMaterial(L"Weapon/LongArrow");
			monsterArrows[i]->ReadMesh(L"Weapon/LongArrow");

			Transform* transform = NULL;

			transform = monsterArrows[i]->AddTransform();
			transform->Position(1, 1, 1);
			transform->RotationDegree(0, 0, 0);
			transform->Scale(1.2f, 0.4f, 1.2f);

			monsterArrows[i]->UpdateTransforms();

			models.push_back(monsterArrows[i]);
		}

		monsterArrowColliders = new ColliderObjectDesc*[RANGEMONSTERCOUNT];
		for (UINT i = 0; i < RANGEMONSTERCOUNT; i++)
		{
			monsterArrowColliders[i] = new ColliderObjectDesc();

			monsterArrowColliders[i]->Init = new Transform();
			monsterArrowColliders[i]->Init->Position(0, 0, 0);
			monsterArrowColliders[i]->Init->RotationDegree(0, 0, 0);
			monsterArrowColliders[i]->Init->Scale(1, 1, 1);

			monsterArrowColliders[i]->Transform = new Transform();
			monsterArrowColliders[i]->Collider = new Collider(monsterArrowColliders[i]->Transform, monsterArrowColliders[i]->Init);
		}
	}
}

void Battle::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void Battle::PlayerAttackControl(UINT attackPlayer)
{
	if (noAttack[attackPlayer] == 1)
	{
		UINT controled = playerControler[attackPlayer]->AnimationState();
		switch (controled)
		{
		case 3:
			player->PlayClip(attackPlayer, 3, 1.5f, 0.3f);
			break;
		case 4:
			player->PlayClip(attackPlayer, 3, 1.5f, 0.3f);
			break;
		case 6:	//활 조준중

			if (playerControler[0]->MoveMode() == true)
			{
				if (Keyboard::Get()->Press('S'))
					player->PlayClip(attackPlayer, 10, 1.0f, 0.3f);
				else if (Keyboard::Get()->Press('W'))
					player->PlayClip(attackPlayer, 11, 1.0f, 0.3f);
				else if (Keyboard::Get()->Press('A'))
					player->PlayClip(attackPlayer, 12, 1.0f, 0.3f);
				else if (Keyboard::Get()->Press('D'))
					player->PlayClip(attackPlayer, 13, 1.0f, 0.3f);
				else
					player->PlayClip(attackPlayer, 7, 1.0f, 0.3f);
			}
			else if (playerControler[0]->MoveMode() == false)
			{
				if (playerControler[0]->ArrowState() == 2)
					player->PlayClip(attackPlayer, 7, 1.0f, 0.3f);
			}

			break;
		}

		playerTime[attackPlayer] += Time::Get()->Delta();
		AttackState[attackPlayer] = PlayerAttackState::Idle;
	}
	else if (noAttack[attackPlayer] == 2)
	{
		UINT equiped = player->CurrEquip();
		switch (equiped)
		{
		case 0:
			playerTime[attackPlayer] = 0;
			player->PlayClip(attackPlayer, 4, 1.5f, 0.3f);
			AttackState[attackPlayer] = PlayerAttackState::PunchAttack;
			if (player->StopClip(attackPlayer, 4))
			{
				for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
					playerToMonster[i] = false;
			}
			break;
		case 1:
			playerTime[attackPlayer] = 0;
			player->PlayClip(attackPlayer, 2, 2.0f, 0.3f);
			AttackState[attackPlayer] = PlayerAttackState::SwordAttack;
			if (player->StopClip(attackPlayer, 2))
			{
				for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
					playerToMonster[i] = false;
			}
			break;
		case 2:
			player->PlayClip(attackPlayer, 8, 1.0f, 0.0f);

			if (player->StopClip(0, 8) == true)
			{
				player->PlayClip(0, 0, 1.0f, 0.0f);
				playerControler[0]->StopDash();
			}

			AttackState[attackPlayer] = PlayerAttackState::DistanceAttack;
			break;
		}
	}

	if (playerTime[attackPlayer] >= 5.0f && (player->CurrEquip() == 1 || player->CurrEquip() == 0))
	{
		noAttack[attackPlayer] = 0;
		playerTime[attackPlayer] = 0;
		AttackState[attackPlayer] = PlayerAttackState::Idle;
		playerControler[0]->StopDash();
	}

	if (AttackState[attackPlayer] == PlayerAttackState::PunchAttack)
	{
		if (player->StopClip(attackPlayer, 4) && noAttack[attackPlayer] == 0)
		{
			for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
				playerToMonster[i] = false;

			noAttack[attackPlayer] = 1;
		}
	}
	if (AttackState[attackPlayer] == PlayerAttackState::SwordAttack)
	{
		if (player->StopClip(attackPlayer, 2) && noAttack[attackPlayer] == 0)
		{
			for (UINT i = 0; i < MONSTERCOUNT + RANGEMONSTERCOUNT; i++)
				playerToMonster[i] = false;

			noAttack[attackPlayer] = 1;
		}
	}
}

void Battle::MonsterAttackControl(UINT attackMonster, Vector3 playerPos)
{
	switch (monsterNoAttack[attackMonster])
	{
	case 1:
		rangeMonster->PlayClip(attackMonster, 4, 1.0f, 0.3f);

		rangeMonsterTime[attackMonster] += Time::Get()->Delta();
		MonsterAttackState[attackMonster] = PlayerAttackState::Idle;
		break;
	case 2:
		rangeMonster->PlayClip(attackMonster, 5, 0.8f, 0.3f);
		MonsterAttackState[attackMonster] = PlayerAttackState::DistanceAttack;

		rangeMonsterControler[attackMonster]->DistanceAttacked(monsterArrows[attackMonster]->GetTransform(0),
			rangeMonsterControler[attackMonster]->AtArrow());

		monsterArrows[attackMonster]->UpdateTransforms();

		if (rangeMonster->StopClip(attackMonster, 5))
		{
			MonsterArrowUpdates(attackMonster);

			monsterArrowColliders[attackMonster]->Collider->Update();
			monsterArrows[attackMonster]->UpdateTransforms();
			rangeMonsterTime[attackMonster] = 5.5f;
		}
		break;
	}

	if (rangeMonsterTime[attackMonster] >= 5.0f)
	{
		monsterNoAttack[attackMonster] = 0;
		rangeMonsterTime[attackMonster] = 0;
		MonsterAttackState[attackMonster] = PlayerAttackState::Idle;
		rangeMonsterControler[attackMonster]->MoveStart();
	}
}

void Battle::RangeAttacked(Vector3 pos)
{
	for (int i = 0; i < PARTICLECOUNT; i++)
	{
		if (explosion[i]->CurrStop() >= 8 && rangeSkillCount < 4)
		{
			rangeSkillCount++;
		}
		if (rangeSkillDelay <= 5.0f)
		{
			explosion[i]->Add(pos);
			explosion[i]->Update();
		}
	}

	static int cameraIn = 0;	//카메라 복귀용
	if (rangeSkillCount < 4)
	{
		Vector3 rangeCamera;

		rangeCamera.x = Math::Random(rangeCameraPos.x - 5, rangeCameraPos.x + 5);
		rangeCamera.y = Math::Random(rangeCameraPos.y - 5, rangeCameraPos.y + 5);
		rangeCamera.z = rangeCameraPos.z;

		Context::Get()->GetCamera()->Position(rangeCamera);
		cameraIn = 1;
		playerControler[0]->Attack(3, player->GetTransform(0), terrain->GetPickedPosition(), Keyboard::Get()->Down('C'));
	}

	if (rangeSkillCount == 4)
	{
		if (cameraIn == 1)
		{
			Context::Get()->GetCamera()->Position(rangeCameraPos);
			cameraIn = 2;
		}
		rangeSkillDelay += Time::Delta();
	}

	if (rangeSkillDelay > 5.0f)
	{
		cameraIn = 0;
		rangeSkillCount = 0;
		rangeSkillDelay = 0;
		rangeSkillOn = false;
	}
}

void Battle::PlayerArrowUpdates()
{
	for (UINT i = 0; i < ARROWCOUNT; i++)
	{
		Transform* transform = playerArrows[i]->GetTransform(0);
		Vector3 pos;
		Vector3 rot;
		Vector3 scale;
		transform->Position(&pos);
		transform->RotationDegree(&rot);
		transform->Scale(&scale);

		if (playerControler[0]->AnimationState() == 6 && noAttack[0] == 1)
		{
			//weaponColliders[0]->Collider->GetTransform()->RotationDegree(&arrowStartRot);
			weaponColliders[0]->Collider->GetTransform()->Position(&arrowStartPos);
			weaponColliders[0]->Collider->GetTransform()->Scale(&arrowStartScale);
			player->GetTransform(0)->RotationDegree(&arrowStartRot);

			pos.x = arrowStartPos.x;
			pos.y = arrowStartPos.y;
			pos.z = arrowStartPos.z;

			//rot.x = arrowStartRot.x + 12.0f;
			//rot.y = arrowStartRot.y + 79.0f;
			rot.x = arrowStartRot.x + 90.0f;
			rot.y = arrowStartRot.y;
			rot.z = arrowStartRot.z;
		}

		playerArrows[i]->GetTransform(0)->Position(pos.x, arrowStartPos.y + scale.y * 3.0f, pos.z);
		playerArrows[i]->GetTransform(0)->RotationDegree(rot);
		playerArrows[i]->GetTransform(0)->Scale(scale);

		playerArrowColliders[i]->Collider->GetTransform()->Position(pos.x, arrowStartPos.y, pos.z);
		playerArrowColliders[i]->Collider->GetTransform()->RotationDegree(rot);
		playerArrowColliders[i]->Collider->GetTransform()->Scale(scale.x, scale.y * 20.0f, scale.z);
	}
}

void Battle::MonsterArrowUpdates(UINT input)
{
	Transform* transform = monsterArrows[input]->GetTransform(0);
	Vector3 pos;
	Vector3 rot;
	Vector3 scale;
	transform->Position(&pos);
	transform->RotationDegree(&rot);
	transform->Scale(&scale);

	static Vector3 arrowStartPos;
	static Vector3 arrowStartRot;
	static Vector3 arrowStartScale;

	if (rangeMonsterControler[input]->AnimationState() == 6 && monsterNoAttack[input] == 1)
	{
		monsterAttackColliders[MONSTERCOUNT + input]->Collider->GetTransform()->Position(&arrowStartPos);
		monsterAttackColliders[MONSTERCOUNT + input]->Collider->GetTransform()->RotationDegree(&arrowStartRot);
		monsterAttackColliders[MONSTERCOUNT + input]->Collider->GetTransform()->Scale(&arrowStartScale);

		pos.x = arrowStartPos.x;
		pos.y = arrowStartPos.y;
		pos.z = arrowStartPos.z;

		rot.x = arrowStartRot.x + 30.0f;
		rot.y = arrowStartRot.y - 52.0f;
		rot.z = arrowStartRot.z;
	}

	monsterArrows[input]->GetTransform(0)->Position(pos.x, arrowStartPos.y + scale.y * 3.0f, pos.z);
	monsterArrows[input]->GetTransform(0)->RotationDegree(rot);
	monsterArrows[input]->GetTransform(0)->Scale(scale);

	monsterArrowColliders[input]->Collider->GetTransform()->Position(pos.x, arrowStartPos.y, pos.z);
	monsterArrowColliders[input]->Collider->GetTransform()->RotationDegree(rot);
	monsterArrowColliders[input]->Collider->GetTransform()->Scale(scale.x, scale.y * 20.0f, scale.z);
}

void Battle::AddSpotLights()
{
	SpotLight light;
	light =
	{
	   Color(0.8f, 0.3f, 0.0f, 1.0f),
	   Color(0.8f, 0.3f, 0.0f, 1.0f),
	   Color(0.8f, 0.3f, 0.0f, 1.0f),
	   Color(0.8f, 0.3f, 0.0f, 1.0f),
	   Vector3(60, 20, 30), 30.0f,
	   Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lights::Get()->AddSpotLight(light);
	//각도는 Range의 +-10정도로 

	light =
	{
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Vector3(100, 20, 30), 30.0f,
	   Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lights::Get()->AddSpotLight(light);
}

#include "stdafx.h"
#include "Player2.h"
#include "./Components/Objects/Transform.h"
#include "./Components/Animation/Animator.h"
#include "./Components/Animation/Clip.h"
#include "./Components/Animation/Sprite.h"
#include "./GameObjects/GamePlay/ObjectX.h"
#include "./GameObjects/GamePlay/p2color.h"
#include "./Components/Bounding/BBox.h"
#include "./Components./Etc/Audio.h"

Player2::Player2()
	: GameObject(L"player2"), diago(false), handBall(false), shooting(false)
	, noMove(0), noStop(false), limitJump(0), posShoot(0), objxFix(false)
	, JumpOrLay(0), playmove(false), noChangein(0), fixi(false), LayUpSpeed(1.0f)
	, LayOrDunk(0), breaker(false), breakpos(0, 0, 0), breakth(0), breakthSpeed(0.0f)
	, breakth2(0), block(false), blockDirection(0), blocked(false), noColi(false)
	, dontmove(false), fake(false)
{
	AddComponent(new BBox);
	objX = new ObjectX();
	p2 = new p2color();
	transform->Position(976, 251);
	transform->Scale(70, 175);
	transform->Pivot(Vector2(0.5f, 0.5f));
	adjScale = transform->Scale();
	//sound
	jumpShootSound = new Audio();
	jumpShootSound->Set(L"JumpShoot.wav", true, false);
	CvDunk = new Audio();
	CvDunk->Set(L"CvDunk.wav", true, false);
	//
	speed = 0.1f;
	mSpeed = 150.0f;
	shootSpeed = 0.7f;

	noChangeFalse();

	anim = new Animator();
	AddComponent(anim);

	FrameData data; Clip* clip;
	//=====================이동===================
	{
		//standdown
		{
			clip = new Clip(L"standdown", AnimationType::Loop);

			data.Time = speed;
			int x = 0;
			int w = x + 64;
			for (int i = 0; i < 23; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 0, w, 167);
				clip->AddFrame(data);
				if (i < 11)
				{
					x += 76;
					w += 76;
				}
				else
				{
					x -= 76;
					w -= 76;
				}
			}
			anim->AddClip(clip);
		}
		//standleft
		{
			clip = new Clip(L"standleft", AnimationType::Loop);

			data.Time = speed;
			int x = 0;
			int w = x + 75;
			for (int i = 0; i < 21; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 168, w, 333);
				clip->AddFrame(data);
				if (i < 10)
				{
					x += 82;
					w += 82;
				}
				else
				{
					x -= 82;
					w -= 82;
				}
			}
			anim->AddClip(clip);
		}
		//standright
		{
			clip = new Clip(L"standright", AnimationType::Loop);

			data.Time = speed;
			int x = 0;
			int w = x + 75;
			for (int i = 0; i < 21; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 334, w, 499);
				clip->AddFrame(data);
				if (i < 10)
				{
					x += 83;
					w += 83;
				}
				else
				{
					x -= 83;
					w -= 83;
				}
			}
			anim->AddClip(clip);
		}
		//standup
		{
			clip = new Clip(L"standup", AnimationType::Loop);

			data.Time = speed;
			int x = 0;
			int w = x + 66;
			for (int i = 0; i < 23; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 500, w, 669);
				clip->AddFrame(data);
				if (i < 11)
				{
					x += 76;
					w += 76;
				}
				else
				{
					x -= 76;
					w -= 76;
				}
			}
			anim->AddClip(clip);
		}
		//moveleft
		{
			clip = new Clip(L"moveleft", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 0;
			int w = x + 102;
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 674, w, 830);
				clip->AddFrame(data);
				x += 108;
				w += 108;
			}
			anim->AddClip(clip);
		}
		//moveright
		{
			clip = new Clip(L"moveright", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 1614;
			int w = x + 102;
			for (int i = 0; i < 16; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-SAM.png", x, 831, w, 987);
				clip->AddFrame(data);
				x -= 108;
				w -= 108;
			}
			anim->AddClip(clip);
		}
		//movedown
		{
			clip = new Clip(L"movedown", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 0;
			int w = x + 95;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 987, w, 1141);
				clip->AddFrame(data);
				x += 103;	//빈칸까지 더한값
				w += 103;
			}
			anim->AddClip(clip);
		}
		//moveup
		{
			clip = new Clip(L"moveup", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 0;
			int w = x + 90;	//캐릭터 최대넓이
			for (int i = 0; i < 15; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 1144, w, 1323);
				clip->AddFrame(data);
				x += 106;	//빈칸까지 더한값
				w += 106;
			}
			anim->AddClip(clip);
		}
		//moverightdown
		{
			clip = new Clip(L"mrd", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 0;
			int w = x + 100;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 1323, w, 1483);
				clip->AddFrame(data);
				x += 108;	//빈칸까지 더한값
				w += 108;
			}
			anim->AddClip(clip);
		}
		//moveleftdown
		{
			clip = new Clip(L"mld", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 1619;
			int w = x + 100;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-SAM.png", x, 1483, w, 1644);
				clip->AddFrame(data);
				x -= 108;	//빈칸까지 더한값
				w -= 108;
			}
			anim->AddClip(clip);
		}
		//moveleftup
		{
			clip = new Clip(L"mlu", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 0;
			int w = x + 98;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-SAM.png", x, 1644, w, 1816);
				clip->AddFrame(data);
				x += 104.5f;	//빈칸까지 더한값
				w += 104.5f;
			}
			anim->AddClip(clip);
		}
		//moverightup
		{
			clip = new Clip(L"mru", AnimationType::Loop);

			data.Time = speed * 0.7f;
			int x = 1563;
			int w = x + 98;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-SAM.png", x, 1816, w, 1988);
				clip->AddFrame(data);
				if (i < 2)
				{
					x -= 100.0f;	//빈칸까지 더한값
					w -= 100.0f;
				}
				else
				{
					x -= 104.5f;	//빈칸까지 더한값
					w -= 104.5f;
				}
			}
			anim->AddClip(clip);
		}
	}

	//====================공들고 이동==============
	{}
	{
		//ballstanddown
		{
			clip = new Clip(L"bsd", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 85;	//캐릭터 최대넓이
			for (int i = 0; i < 23; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 1, w, 162);
				clip->AddFrame(data);
				if (i == 2)
				{
					x += 88;	//빈칸까지 더한값
					w += 88;
				}
				else if (i < 11)
				{
					x += 90;	//빈칸까지 더한값
					w += 90;
				}
				else if (i == 19)
				{
					x -= 88;	//빈칸까지 더한값
					w -= 88;
				}
				else if (i < 23)
				{
					x -= 90;	//빈칸까지 더한값
					w -= 90;
				}
			}
			anim->AddClip(clip);
		}
		//ballstandleft
		{
			clip = new Clip(L"bsl", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 55;	//캐릭터 최대넓이
			for (int i = 0; i < 19; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 178, w, 341);
				clip->AddFrame(data);
				if (i < 9)
				{
					x += 57;	//빈칸까지 더한값
					w += 57;
				}
				else if (i < 19)
				{
					x -= 57;	//빈칸까지 더한값
					w -= 57;
				}
			}
			anim->AddClip(clip);
		}
		//ballstandright
		{
			clip = new Clip(L"bsr", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 55;	//캐릭터 최대넓이
			for (int i = 0; i < 19; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 345, w, 508);
				clip->AddFrame(data);
				if (i < 9)
				{
					x += 57;	//빈칸까지 더한값
					w += 57;
				}
				else if (i < 19)
				{
					x -= 57;	//빈칸까지 더한값
					w -= 57;
				}
			}
			anim->AddClip(clip);
		}
		//ballstandup
		{
			clip = new Clip(L"bsu", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 83;	//캐릭터 최대넓이
			for (int i = 0; i < 21; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 518, w, 677);
				clip->AddFrame(data);

				if (i < 10)
				{
					x += 87;	//빈칸까지 더한값
					w += 87;
				}
				else if (i < 21)
				{
					x -= 87;	//빈칸까지 더한값
					w -= 87;
				}
			}
			anim->AddClip(clip);
		}
		//ballmovedown
		{
			clip = new Clip(L"bmd", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 90;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-BSAM.png", x, 677, w, 847);
				clip->AddFrame(data);

				x += 99;	//빈칸까지 더한값
				w += 99;
			}
			anim->AddClip(clip);
		}
		//ballmoveright
		{
			clip = new Clip(L"bmr", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 118;	//캐릭터 최대넓이
			for (int i = 0; i < 17; i++)
			{
				data.Image = new Sprite(L"C-BSAM.png", x, 848, w, 1015);
				clip->AddFrame(data);

				x += 122;	//빈칸까지 더한값
				w += 122;
			}
			anim->AddClip(clip);
		}
		//ballmoveleft
		{
			clip = new Clip(L"bml", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 2081;
			int w = x + 118;	//캐릭터 최대넓이
			for (int i = 0; i < 17; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 1016, w, 1183);
				clip->AddFrame(data);
				x -= 130;	//빈칸까지 더한값
				w -= 130;
			}
			anim->AddClip(clip);
		}
		//ballmoveup
		{
			clip = new Clip(L"bmu", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 80;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-BSAM.png", x, 1184, w, 1361);
				clip->AddFrame(data);
				x += 92;	//빈칸까지 더한값
				w += 92;
			}
			anim->AddClip(clip);
		}
		//ballmoverightdown
		{
			clip = new Clip(L"bmrd", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{

				data.Image = new Sprite(L"C-BSAM.png", x, 1362, w, 1527);
				clip->AddFrame(data);
				x += 117;	//빈칸까지 더한값
				w += 117;
			}
			anim->AddClip(clip);
		}
		//ballmoveleftdown
		{
			clip = new Clip(L"bmld", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 1749;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 1533, w, 1693);
				clip->AddFrame(data);
				x -= 117;	//빈칸까지 더한값
				w -= 117;
			}
			anim->AddClip(clip);
		}
		//ballmoveleftup
		{
			clip = new Clip(L"bmlu", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				data.Image = new Sprite(L"C-BSAM.png", x, 1693, w, 1876);
				clip->AddFrame(data);
				x += 120;	//빈칸까지 더한값
				w += 120;
			}
			anim->AddClip(clip);
		}
		//ballmoverightup
		{
			clip = new Clip(L"bmru", AnimationType::Loop);

			data.Time = speed * 0.8f;
			int x = 1800;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 16; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"C-BSAM.png", x, 1876, w, 2059);
				clip->AddFrame(data);
				x -= 120;	//빈칸까지 더한값
				w -= 120;
			}
			anim->AddClip(clip);
		}
	}
	//====================슛===================
	{}
	{
		//leftdiagoshoot
		{
			clip = new Clip(L"lds", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 0;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 22; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 0, w, 197);
				clip->AddFrame(data);
				x += 119;	//빈칸까지 더한값
				w += 119;
			}
			anim->AddClip(clip);
		}
		//fakeleftdiagoshoot
		{
			clip = new Clip(L"flds", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 476;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 5; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 0, w, 197);
				clip->AddFrame(data);
				x -= 119;	//빈칸까지 더한값
				w -= 119;
			}
			anim->AddClip(clip);
		}
		//rightdiagoshoot
		{
			clip = new Clip(L"rds", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 2501;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 21; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 197, w, 394);
				clip->AddFrame(data);
				x -= 120.5f;	//빈칸까지 더한값
				w -= 120.5f;
			}
			anim->AddClip(clip);
		}
		//fakerightdiagoshoot
		{
			clip = new Clip(L"frds", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 2010;
			int w = x + 103;	//캐릭터 최대넓이
			for (int i = 0; i < 5; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 197, w, 394);
				clip->AddFrame(data);
				x += 119;	//빈칸까지 더한값
				w += 119;
			}
			anim->AddClip(clip);
		}
		//leftsideshoot
		{
			clip = new Clip(L"lss", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 2415;
			int w = x + 86;	//캐릭터 최대넓이
			for (int i = 0; i < 22; i++)
			{
				//if (x < 0) x = 0;
				data.Image = new Sprite(L"Shoot.png", x, 394, w, 598);
				clip->AddFrame(data);
				x -= 115;	//빈칸까지 더한값
				w -= 115;
			}
			anim->AddClip(clip);
		}
		//fakeleftsideshoot
		{
			clip = new Clip(L"flss", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 1955;
			int w = x + 86;	//캐릭터 최대넓이
			for (int i = 0; i < 5; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 394, w, 598);
				clip->AddFrame(data);
				x += 115;	//빈칸까지 더한값
				w += 115;
			}
			anim->AddClip(clip);
		}
		//rightsideshoot
		{
			clip = new Clip(L"rss", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 0;
			int w = x + 86;	//캐릭터 최대넓이
			for (int i = 0; i < 22; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 598, w, 802);
				clip->AddFrame(data);
				x += 114;	//빈칸까지 더한값
				w += 114;
			}
			anim->AddClip(clip);
		}
		//fakerightsideshoot
		{
			clip = new Clip(L"frss", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 460;
			int w = x + 86;	//캐릭터 최대넓이
			for (int i = 0; i < 5; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 598, w, 802);
				clip->AddFrame(data);
				x -= 115;	//빈칸까지 더한값
				w -= 115;
			}
			anim->AddClip(clip);
		}
		//centershoot
		{
			clip = new Clip(L"cs", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 0;
			int w = x + 86;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 802, w, 1029);
				clip->AddFrame(data);
				x += 104.8f;	//빈칸까지 더한값
				w += 104.8f;
			}
			anim->AddClip(clip);
		}
		//fakecentershoot
		{
			clip = new Clip(L"fcs", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 420;
			int w = x + 86;	//캐릭터 최대넓이
			for (int i = 0; i < 5; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 802, w, 1029);
				clip->AddFrame(data);
				x -= 105;	//빈칸까지 더한값
				w -= 105;
			}
			anim->AddClip(clip);
		}
		//centerlayupshoot
		{
			clip = new Clip(L"cls", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 0;
			int w = x + 97;	//캐릭터 최대넓이
			for (int i = 0; i < 20; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 1029, w, 1248);
				clip->AddFrame(data);
				x += 109;	//빈칸까지 더한값
				w += 109;
			}
			anim->AddClip(clip);
		}
		//leftlayupshoot
		{
			clip = new Clip(L"lls", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 1;
			int w = x + 98;	//캐릭터 최대넓이
			for (int i = 0; i < 18; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 1248, w, 1460);
				clip->AddFrame(data);
				x += 113;	//빈칸까지 더한값
				w += 113;
			}
			anim->AddClip(clip);
		}
		//rightlayupshoot
		{
			clip = new Clip(L"rls", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 1895;
			int w = x + 98;	//캐릭터 최대넓이
			for (int i = 0; i < 18; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"Shoot.png", x, 1460, w, 1672);
				clip->AddFrame(data);
				x -= 112;	//빈칸까지 더한값
				w -= 112;
			}
			anim->AddClip(clip);
		}
		//rightdunkshoot
		{
			clip = new Clip(L"rdunk", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 1;
			int w = x + 88;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				data.Image = new Sprite(L"Shoot.png", x, 1672, w, 1863);
				clip->AddFrame(data);
				x += 114;	//빈칸까지 더한값
				w += 114;
			}
			anim->AddClip(clip);
		}
		//leftdunkshoot
		{
			clip = new Clip(L"ldunk", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 2590;
			int w = x + 88;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				if (x < 0) x = 0;
				data.Image = new Sprite(L"Shoot.png", x, 1863, w, 2054);
				clip->AddFrame(data);
				x -= 113;	//빈칸까지 더한값
				w -= 113;
			}
			anim->AddClip(clip);
		}
		//middledunkshoot
		{
			clip = new Clip(L"mdunk", AnimationType::End);

			data.Time = speed * shootSpeed;
			int x = 0;
			int w = x + 119;	//캐릭터 최대넓이
			for (int i = 0; i < 25; i++)
			{
				if (i == 22) x = 2622;
				data.Image = new Sprite(L"Shoot.png", x, 2054, w, 2246);
				clip->AddFrame(data);
				x += 119;	//빈칸까지 더한값
				w += 119;
			}
			anim->AddClip(clip);
		}

	}
	//====================돌파=====================
	{}
	{
		//centerleftbreakthrough
		{
			clip = new Clip(L"clbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 129;	//캐릭터 최대넓이
			for (int i = 0; i < 10; i++)
			{
				data.Image = new Sprite(L"breakthrough.png", x, 0, w, 174);
				clip->AddFrame(data);
				x += 143;	//빈칸까지 더한값
				w += 143;
			}
			anim->AddClip(clip);
		}
		//centerrightbreakthrough
		{
			clip = new Clip(L"crbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 1259;
			int w = x + 129;	//캐릭터 최대넓이
			for (int i = 0; i < 10; i++)
			{
				if (x <= 0) x = 0;
				data.Image = new Sprite(L"breakthrough.png", x, 174, w, 348);
				clip->AddFrame(data);
				x -= 140;	//빈칸까지 더한값
				w -= 140;
			}
			anim->AddClip(clip);
		}
		//leftdiagoleftbreakthrough
		{
			clip = new Clip(L"ldlbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 118;	//캐릭터 최대넓이
			for (int i = 0; i < 12; i++)
			{
				data.Image = new Sprite(L"breakthrough.png", x, 348, w, 519);
				clip->AddFrame(data);
				x += 130;	//빈칸까지 더한값
				w += 130;
			}
			anim->AddClip(clip);
		}
		//leftdiagorightbreakthrough
		{
			clip = new Clip(L"ldrbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 162;	//캐릭터 최대넓이
			for (int i = 0; i < 11; i++)
			{
				if (i == 3) w = x + 248;
				if (i == 4) {
					x = 734;
					w = x + 162;
				}
				if (i == 8) w = x + 235;
				if (i == 9) {
					x = 1617;
					w = x + 162;
				}
				data.Image = new Sprite(L"breakthrough.png", x, 519, w, 686);
				clip->AddFrame(data);
				x += 162;
				w += 162;
			}
			anim->AddClip(clip);
		}
		//rightdiagorightbreakthrough
		{
			clip = new Clip(L"rdrbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 118;	//캐릭터 최대넓이
			for (int i = 0; i < 12; i++)
			{
				data.Image = new Sprite(L"breakthrough.png", x, 686, w, 857);
				clip->AddFrame(data);
				x += 151;	//빈칸까지 더한값
				w += 151;
			}
			anim->AddClip(clip);
		}
		//rightdiagoleftbreakthrough
		{
			clip = new Clip(L"rdlbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 162;	//캐릭터 최대넓이
			for (int i = 0; i < 11; i++)
			{
				if (i == 3) w = x + 248;
				if (i == 4) {
					x = 734;
					w = x + 162;
				}
				if (i == 8) w = x + 235;
				if (i == 9) {
					x = 1617;
					w = x + 162;
				}
				data.Image = new Sprite(L"breakthrough.png", x, 857, w, 1024);
				clip->AddFrame(data);
				x += 162;	//빈칸까지 더한값
				w += 162;
			}
			anim->AddClip(clip);
		}
		//leftsideleftbreakthrough
		{
			clip = new Clip(L"lslbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 145;	//캐릭터 최대넓이
			for (int i = 0; i < 10; i++)
			{
				if (i == 3) w = x + 227;
				if (i == 4)
				{
					x = 662;
					w = x + 145;
				}
				data.Image = new Sprite(L"breakthrough.png", x, 1024, w, 1200);
				clip->AddFrame(data);
				x += 145;	//빈칸까지 더한값
				w += 145;
			}
			anim->AddClip(clip);
		}
		//leftsiderightbreakthrough
		{
			clip = new Clip(L"lsrbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 137;	//캐릭터 최대넓이
			for (int i = 0; i < 12; i++)
			{
				if (i == 5) {
					x = 689;
					w = x + 135;
				}
				if (i == 7)
					x = 964;
				if (i == 9)
					x = 1240;
				data.Image = new Sprite(L"breakthrough.png", x, 1200, w, 1361);
				clip->AddFrame(data);
				x += 138;	//빈칸까지 더한값
				w += 138;
			}
			anim->AddClip(clip);
		}
		//rightsiderightbreakthrough
		{
			clip = new Clip(L"rsrbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 145;	//캐릭터 최대넓이
			for (int i = 0; i < 10; i++)
			{
				if (i == 3) w = x + 227;
				if (i == 4)
				{
					x = 662;
					w = x + 145;
				}
				data.Image = new Sprite(L"breakthrough.png", x, 1361, w, 1537);
				clip->AddFrame(data);
				x += 145;	//빈칸까지 더한값
				w += 145;
			}
			anim->AddClip(clip);
		}
		//rightsideleftbreakthrough
		{
			clip = new Clip(L"rslbt", AnimationType::End);

			data.Time = speed * 0.8f;
			int x = 0;
			int w = x + 137;	//캐릭터 최대넓이
			for (int i = 0; i < 12; i++)
			{
				data.Image = new Sprite(L"breakthrough.png", x, 1537, w, 1698);
				clip->AddFrame(data);
				x += 146;	//빈칸까지 더한값
				w += 146;
			}
			anim->AddClip(clip);
		}
	}
	//====================블락=====================
	{}
	{
		//centerblock
		{
			clip = new Clip(L"cblock", AnimationType::End);

			data.Time = speed * 0.5f;
			int x = 0;
			int w = x + 98;	//캐릭터 최대넓이
			for (int i = 0; i < 28; i++)
			{
				if (i == 8) x = 837;
				if (i == 14) x = 1465;
				if (i == 20) {
					x = 2093;
					w = x + 91;
				}
				if (i == 24) x = 2512;
				data.Image = new Sprite(L"block.png", x, 0, w, 206);
				clip->AddFrame(data);
				x += 105;	//빈칸까지 더한값
				w += 105;
			}
			anim->AddClip(clip);
		}
		//rightdiagoblock
		{
			clip = new Clip(L"rdblock", AnimationType::End);

			data.Time = speed * 0.5f;
			int x = 0;
			int w = x + 101;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				data.Image = new Sprite(L"block.png", x, 206, w, 405);
				clip->AddFrame(data);
				x += 123;	//빈칸까지 더한값
				w += 123;
			}
			anim->AddClip(clip);
		}
		//leftdiagoblock
		{
			clip = new Clip(L"ldblock", AnimationType::End);

			data.Time = speed * 0.5f;
			int x = 0;
			int w = x + 101;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				data.Image = new Sprite(L"block.png", x, 405, w, 604);
				clip->AddFrame(data);
				x += 123;	//빈칸까지 더한값
				w += 123;
			}
			anim->AddClip(clip);
		}
		//rightblock
		{
			clip = new Clip(L"rblock", AnimationType::End);

			data.Time = speed * 0.5f;
			int x = 0;
			int w = x + 178;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				if (i == 8) x = 1597;
				data.Image = new Sprite(L"block.png", x, 605, w, 831);
				clip->AddFrame(data);
				x += 200;	//빈칸까지 더한값
				w += 200;
			}
			anim->AddClip(clip);
		}
		//leftblock
		{
			clip = new Clip(L"lblock", AnimationType::End);

			data.Time = speed * 0.5f;
			int x = 4585;
			int w = x + 178;	//캐릭터 최대넓이
			for (int i = 0; i < 24; i++)
			{
				if (x <= 0)	x = 0;
				if (i == 8) x = 2990;
				data.Image = new Sprite(L"block.png", x, 831, w, 1058);
				clip->AddFrame(data);
				x -= 200;	//빈칸까지 더한값
				w -= 200;
			}
			anim->AddClip(clip);
		}
	}
	anim->Play(L"standdown");
	vec = transform->Position();

}


Player2::~Player2()
{
}

void Player2::Update()
{
	__super::Update();//__super 바로 윗 부모 
	if (noMove == 0)
		objxFix = false;
	if (objxFix == false)
		objX->Pos(pos2, 2);
	objX->Update();
	pos2 = transform->Position();
	objXPos = objX->transform->Position();
	p2->transform->Position(objXPos);
	p2->Update();
	if (breaker == false)
		FixPos = objX->transform->Position();
	scaleSave = transform->Scale();
	//==============위치별 크기조절==============================
	{
		int posY = 470 - objXPos.y;
		switch (posY / 10)
		{
		case 0:
			adjScale.x = 55.5f; break;
		case 1:
			adjScale.x = 56; break;
		case 2:
			adjScale.x = 56.5f; break;
		case 3:
			adjScale.x = 57; break;
		case 4:
			adjScale.x = 57.5f; break;
		case 5:
			adjScale.x = 58; break;
		case 6:
			adjScale.x = 58.5f; break;
		case 7:
			adjScale.x = 59; break;
		case 8:
			adjScale.x = 59.5f; break;
		case 9:
			adjScale.x = 60; break;
		case 10:
			adjScale.x = 60.5f; break;
		case 11:
			adjScale.x = 61; break;
		case 12:
			adjScale.x = 61.5f; break;
		case 13:
			adjScale.x = 62; break;
		case 14:
			adjScale.x = 62.5f; break;
		case 15:
			adjScale.x = 63; break;
		case 16:
			adjScale.x = 63.5f; break;
		case 17:
			adjScale.x = 64; break;
		case 18:
			adjScale.x = 64.5f; break;
		case 19:
			adjScale.x = 65; break;
		case 20:
			adjScale.x = 65.5f; break;
		case 21:
			adjScale.x = 66; break;
		case 22:
			adjScale.x = 66.5f; break;
		case 23:
			adjScale.x = 67; break;
		case 24:
			adjScale.x = 67.5f; break;
		case 25:
			adjScale.x = 68; break;
		case 26:
			adjScale.x = 68.5f; break;
		case 27:
			adjScale.x = 69; break;
		case 28:
			adjScale.x = 69.5f; break;
		case 29:
			adjScale.x = 70; break;
		case 30:
			adjScale.x = 70.5f; break;
		case 31:
			adjScale.x = 71; break;
		case 32:
			adjScale.x = 71.5f; break;
		case 33:
			adjScale.x = 72; break;
		case 34:
			adjScale.x = 72.5f; break;
		case 35:
			adjScale.x = 73; break;
		case 36:
			adjScale.x = 73.5f; break;
		default:
			if (objXPos.y < 100)
				adjScale.x = 73.5f; break;
		}
		adjScale.y = adjScale.x * 2.5;
	}

	//========================움직임============================
	if (handBall == false && noMove == 0 && block == false)
	{
		//서있기
		if (Input::Get()->KeyUp('D'))
		{
			transform->Scale(adjScale.x, adjScale.y);
			anim->Play(L"standleft");
			diago = false;
		}
		if (Input::Get()->KeyUp('G'))
		{
			transform->Scale(adjScale.x, adjScale.y);
			anim->Play(L"standright");
			diago = false;
		}
		if (Input::Get()->KeyUp('F'))
		{
			transform->Scale(adjScale.x, adjScale.y);
			anim->Play(L"standdown");
			diago = false;
		}
		if (Input::Get()->KeyUp('R'))
		{
			transform->Scale(adjScale.x, adjScale.y);
			anim->Play(L"standup");
			diago = false;
		}
		//대각선 이동
		if (Input::Get()->Key('G') &&
			(Input::Get()->Key('F')))
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y);
			anim->Play(L"mrd");
			pos2Save.y = pos2.y;
			pos2.x += mSpeed * 0.6f * g_ETime;
			pos2.y -= mSpeed * 0.6f * g_ETime;
			transform->Position(pos2);
			diago = true;
		}
		else if (Input::Get()->Key('D') &&
			(Input::Get()->Key('F')))
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y);
			anim->Play(L"mld");
			pos2Save.y = pos2.y;
			pos2.x -= mSpeed * 0.6f * g_ETime;
			pos2.y -= mSpeed * 0.6f * g_ETime;
			transform->Position(pos2);
			diago = true;
		}
		else if (Input::Get()->Key('D') &&
			(Input::Get()->Key('R')))
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y);
			anim->Play(L"mlu");
			pos2Save.y = pos2.y;
			pos2.x -= mSpeed * 0.6f * g_ETime;
			pos2.y += mSpeed * 0.6f * g_ETime;
			transform->Position(pos2);
			diago = true;
		}
		else if (Input::Get()->Key('G') &&
			(Input::Get()->Key('R')))
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y);
			anim->Play(L"mru");
			pos2Save.y = pos2.y;
			pos2.x += mSpeed * 0.6f * g_ETime;
			pos2.y += mSpeed * 0.6f * g_ETime;
			transform->Position(pos2);
			diago = true;
		}
		//상하좌우 이동
		if (diago == false)
		{
			if (Input::Get()->Key('D'))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y);
				anim->Play(L"moveleft");
				pos2.x -= mSpeed * g_ETime;
				transform->Position(pos2);
			}
			else if (Input::Get()->Key('G'))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y);
				anim->Play(L"moveright");
				pos2.x += mSpeed * g_ETime;
				transform->Position(pos2);
			}
			else if (Input::Get()->Key('F'))
			{
				transform->Scale(adjScale.x + 33.0f, adjScale.y);
				anim->Play(L"movedown");
				pos2Save.y = pos2.y;
				pos2.y -= mSpeed * 0.6f * g_ETime;
				transform->Position(pos2);
			}
			else if (Input::Get()->Key('R'))
			{
				transform->Scale(adjScale.x + 33.0f, adjScale.y);
				anim->Play(L"moveup");
				pos2Save.y = pos2.y;
				pos2.y += mSpeed * 0.6f * g_ETime;
				transform->Position(pos2);
			}
		}
	}
	//=========================블락=============================
	if (handBall == false && dontmove == false)
	{
		Block();
	}
	//======================공있을때=========================
	if (handBall == true)
	{
		//공들고움직임
		if (noMove == 0 && breaker == false)
		{
			if (Input::Get()->KeyUp('F'))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y);
				anim->Play(L"bsd");
				diago = false;
			}
			if (Input::Get()->KeyUp('D'))
			{
				transform->Scale(adjScale.x - 10.0f, adjScale.y);
				anim->Play(L"bsl");
				diago = false;
			}
			if (Input::Get()->KeyUp('G'))
			{
				transform->Scale(adjScale.x - 10.0f, adjScale.y);
				anim->Play(L"bsr");
				diago = false;
			}
			if (Input::Get()->KeyUp('R'))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y);
				anim->Play(L"bsu");
				diago = false;
			}
			//대각선 이동
			if (Input::Get()->Key('G') &&
				(Input::Get()->Key('F')))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y);
				anim->Play(L"bmrd");
				pos2Save.y = pos2.y;
				pos2.x += mSpeed * 0.6f * g_ETime;
				pos2.y -= mSpeed * 0.6f * g_ETime;
				transform->Position(pos2);
				diago = true;
			}
			else if (Input::Get()->Key('D') &&
				(Input::Get()->Key('F')))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y);
				anim->Play(L"bmld");
				pos2Save.y = pos2.y;
				pos2.x -= mSpeed * 0.6f * g_ETime;
				pos2.y -= mSpeed * 0.6f * g_ETime;
				transform->Position(pos2);
				diago = true;
			}
			else if (Input::Get()->Key('D') &&
				(Input::Get()->Key('R')))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y + 10.0f);
				anim->Play(L"bmlu");
				pos2Save.y = pos2.y;
				pos2.x -= mSpeed * 0.6f * g_ETime;
				pos2.y += mSpeed * 0.6f * g_ETime;
				transform->Position(pos2);
				diago = true;
			}
			else if (Input::Get()->Key('G') &&
				(Input::Get()->Key('R')))
			{
				transform->Scale(adjScale.x + 25.0f, adjScale.y + 10.0f);
				anim->Play(L"bmru");
				pos2Save.y = pos2.y;
				pos2.x += mSpeed * 0.6f * g_ETime;
				pos2.y += mSpeed * 0.6f * g_ETime;
				transform->Position(pos2);
				diago = true;
			}
			//상하좌우
			if (diago == false)
			{
				if (Input::Get()->Key('D'))
				{
					transform->Scale(adjScale.x + 45.0f, adjScale.y);
					anim->Play(L"bml");
					pos2.x -= mSpeed * g_ETime;
					transform->Position(pos2);
				}
				else if (Input::Get()->Key('G'))
				{
					transform->Scale(adjScale.x + 45.0f, adjScale.y);
					anim->Play(L"bmr");
					pos2.x += mSpeed * g_ETime;
					transform->Position(pos2);
				}
				else if (Input::Get()->Key('F'))
				{
					transform->Scale(adjScale.x + 20.0f, adjScale.y);
					anim->Play(L"bmd");
					pos2Save.y = pos2.y;
					pos2.y -= mSpeed * 0.6f * g_ETime;
					transform->Position(pos2);
				}
				else if (Input::Get()->Key('R'))
				{
					transform->Scale(adjScale.x + 13.0f, adjScale.y + 10.0f);
					anim->Play(L"bmu");
					pos2Save.y = pos2.y;
					pos2.y += mSpeed * 0.6f * g_ETime;
					transform->Position(pos2);
				}
			}
		}
		//슛 과 돌파
		if(dontmove == false)
		{
			//골대 바로밑에서 끊기는 오류 수정
			if (playmove == true && (FixPos.y >= 245) && (FixPos.x >= 450) && (FixPos.x <= 826))
			{
				if (breaker == false)
					LayupDunk(posShoot);
				Breakthrough();
			}
			//왼쪽인사이드
			if (playmove == false && (FixPos.y >= 400) && (FixPos.x >= 450) && (FixPos.x <= 575))
			{
				posShoot = 1;
				iLeRiPos = 1;
				noChangein = 0;
				if (breaker == false)
					LayupDunk(posShoot);
				Breakthrough();
			}
			//오른쪽인사이드
			if (playmove == false && (FixPos.y >= 400) && (FixPos.x >= 701) && (FixPos.x <= 826))
			{
				posShoot = 1;
				iLeRiPos = 2;
				noChangein = 1;
				if (breaker == false)
					LayupDunk(posShoot);
				Breakthrough();
			}
			//왼쪽인대각선
			if (playmove == false && (FixPos.y >= 245) && (FixPos.y < 400) && (FixPos.x >= 450) && (FixPos.x <= 575))
			{
				posShoot = 2;
				iLeRiPos = 1;
				noChangein = 2;
				if (breaker == false)
					LayupDunk(posShoot);
				Breakthrough();
			}
			//오른쪽인대각선
			if (playmove == false && (FixPos.y >= 245) && (FixPos.y < 400) && (FixPos.x >= 701) && (FixPos.x <= 826))
			{
				posShoot = 2;
				iLeRiPos = 2;
				noChangein = 3;
				if (breaker == false)
					LayupDunk(posShoot);
				Breakthrough();
			}
			//가운데인
			if (playmove == false && (FixPos.y >= 245) && (FixPos.y < 400) && (FixPos.x <= 701) && (FixPos.x >= 575))
			{
				posShoot = 3;
				iLeRiPos = 3;
				noChangein = 4;
				if (breaker == false)
					LayupDunk(posShoot);
				Breakthrough();
			}
			//왼쪽사이드
			if ((FixPos.y >= 400) && (FixPos.x < 450))
			{
				posShoot = 1;
				iLeRiPos = 1;
				if (breaker == false)
					JumpShoot(posShoot);
				Breakthrough();
			}
			//오른쪽사이드
			if ((FixPos.y >= 400) && (FixPos.x > 826))
			{
				posShoot = 1;
				iLeRiPos = 2;
				if (breaker == false)
					JumpShoot(posShoot);
				Breakthrough();
			}
			//왼쪽대각선
			if ((FixPos.y < 400) && (FixPos.x < 450))
			{
				posShoot = 2;
				iLeRiPos = 1;
				if (breaker == false)
					JumpShoot(posShoot);
				Breakthrough();
			}
			//오른쪽대각선
			if ((FixPos.y < 400) && (FixPos.x > 826))
			{
				posShoot = 2;
				iLeRiPos = 2;
				if (breaker == false)
					JumpShoot(posShoot);
				Breakthrough();
			}
			//가운데
			if ((FixPos.y < 245) && (FixPos.x <= 826) && (FixPos.x >= 450))
			{
				posShoot = 3;
				iLeRiPos = 3;
				if (breaker == false)
					JumpShoot(posShoot);
				Breakthrough();
			}
		}
	}
}

void Player2::Render()
{
	objX->Render();
	p2->Render();
	__super::Render();
	Vector3 goalPos = Vector3(638, 1000, 0); //골대 위치
	/*ImGui::Begin("Debug");
	{
		ImGui::Text("Distince : %.2f", (D3DXVec3Length(&(objXPos - goalPos))));
		ImGui::Text("animSpeed : %.2f", speed);
		ImGui::Text("pScaleX : %.2f pScaleY : %.2f", scaleSave.x, scaleSave.y);
	}
	ImGui::End();*/
}

void Player2::JumpControl(int frame)
{
	if (dontmove == false)
	{
		Vector3 objxScale = objX->transform->Scale();
		//점프제어
		if (JumpOrLay == 1)	//점프슛
		{
			hJump = 15.0f;
			if (frame == 6 && limitJump == 0)
			{
				limitJump = 1;
				jumpShootSound->Play();
			}
			if (frame == 7 && limitJump == 7)
			{
				limitJump = 2;
			}
			if (frame == 8 && limitJump == 8)
			{
				limitJump = 3;
			}
			if (frame == 9 && limitJump == 9)
			{
				limitJump = 4;
			}
			if (frame == 10 && limitJump == 10)
			{
				limitJump = 5;
			}
			if (frame == 11 && limitJump == 11)
			{
				limitJump = 6;
			}
			switch (limitJump)
			{
			case 1:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 7;
				break;
			case 2:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 8;
				break;
			case 3:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 9;
				break;
			case 4:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 10;
				break;
			case 5:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 11;
				break;
			case 6:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 12;
				break;
			}
		}
		if (JumpOrLay == 2 && fixi == true)	//레이업
		{
			if ((D3DXVec3Length(&(objXPos - Vector3(638.0f, 475.0f, 0))) > 20.0f))
			{
				LayUpSpeed = 2.5f;
				Vector3 temp;
				temp = Vector3(638.0f, 475.0f, 0) - objXPos;
				D3DXVec3Normalize(&temp, &temp);
				temp.x += g_ETime;
				temp.y += g_ETime;
				pos2.x += temp.x * LayUpSpeed;
				pos2.y += temp.y * LayUpSpeed;
				objXPos.x += temp.x * LayUpSpeed;
				objXPos.y += temp.y * LayUpSpeed;
				objX->transform->Position(objXPos);
			}
			hJump = 10.0f;
			switch (frame)
			{
			case 0:
				if (limitJump == 0)
					limitJump = -1;
				break;
			case 1:
				if (limitJump == -2)
					limitJump = 0;
				break;

			}
			switch (limitJump)
			{
			case -1:
				if (Dunkball() == 2)
					CvDunk->Play();
				if ((anim->IsPlay(L"mdunk") || anim->IsPlay(L"cls")) == true)
					pos2.x += 20.0f;
				pos2.y += 25.0f;
				limitJump = -2;
				break;
			}
			switch (frame)
			{
			case 4:
				if (limitJump == 0)
					limitJump = 1;
				break;
			case 5:
				if (limitJump == 11)
					limitJump = 2;
				break;
			case 6:
				if (limitJump == 12)
					limitJump = 3;
				break;
			case 7:
				if (limitJump == 13)
					limitJump = 4;
				break;
			case 8:
				if (limitJump == 14)
					limitJump = 5;
				break;
			case 9:
				if (limitJump == 15)
					limitJump = 6;
				break;
			case 10:
				if (limitJump == 16)
					limitJump = 7;
				break;
			case 11:
				if (limitJump == 17)
					limitJump = 8;
				break;
			case 12:
				if (limitJump == 18)
					limitJump = 9;
				break;
			case 13:
				if (limitJump == 19)
					limitJump = 10;
				break;
			}
			switch (limitJump)
			{
			case 1:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 11;
				break;
			case 2:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 12;
				break;
			case 3:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 13;
				break;
			case 4:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 14;
				break;
			case 5:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 15;
				break;
			case 6:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				SlideLayDunk();
				limitJump = 16;
				break;
			case 7:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				SlideLayDunk();
				limitJump = 17;
				break;
			case 8:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				SlideLayDunk();
				limitJump = 18;
				break;
			case 9:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				SlideLayDunk();
				limitJump = 19;
				break;
			case 10:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				SlideLayDunk();
				limitJump = 20;
				fixi = false;
				break;
			}
		}
		if (block == true)	//블록
		{
			blocked = false;
			hJump = 15.0f;
			switch (frame)
			{
			case 0:
				if (limitJump == 0)
					limitJump = -1;
				break;
			case 1:
				if (limitJump == -2)
					limitJump = 0;
				break;

			}
			switch (limitJump)
			{
			case -1:
				if (blockDirection == 1 || blockDirection == 2)
					pos2.x += 20.0f;
				if (blockDirection == 4)
					pos2.x += 40.0f;
				pos2.y += 25.0f;
				limitJump = -2;
				break;
			}
			switch (frame)
			{
			case 3:
				if (limitJump == 0)
					limitJump = 1;
				break;
			case 4:
				if (limitJump == 13)
					limitJump = 2;
				break;
			case 5:
				if (limitJump == 14)
					limitJump = 3;
				break;
			case 6:
				if (limitJump == 15)
					limitJump = 4;
				break;
			case 7:
				if (limitJump == 16)
					limitJump = 5;
				break;
			case 8:
				if (limitJump == 17)
					limitJump = 6;
				break;
			case 14:
				if (limitJump == 18)
					limitJump = 7;
				break;
			case 15:
				if (limitJump == 19)
					limitJump = 8;
				break;
			case 16:
				if (limitJump == 20)
					limitJump = 9;
				break;
			case 17:
				if (limitJump == 21)
					limitJump = 10;
				break;
			case 18:
				if (limitJump == 22)
					limitJump = 11;
				break;
			case 19:
				if (limitJump == 23)
					limitJump = 12;
				break;
			}
			switch (limitJump)
			{
			case 1:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 13;
				break;
			case 2:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 14;
				break;
			case 3:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 15;
				break;
			case 4:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 16;
				break;
			case 5:
				pos2.y += hJump;
				objxScale.x -= 7.0f;
				objxScale.y -= 1.5f;
				limitJump = 17;
				blockPos = Vector3(pos2.x, pos2.y + (scaleSave.y / 2), 0);
				blocked = true;
				break;
			case 6:
				pos2.y += hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 18;
				blockPos = Vector3(pos2.x, pos2.y + (scaleSave.y / 2), 0);
				blocked = true;
				break;
			case 7:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 19;
				blockPos = Vector3(pos2.x, pos2.y + (scaleSave.y / 2), 0);
				blocked = true;
				break;
			case 8:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 20;
				blockPos = Vector3(pos2.x, pos2.y + (scaleSave.y / 2), 0);
				blocked = true;
				break;
			case 9:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 21;
				break;
			case 10:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 22;
				break;
			case 11:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 23;
				break;
			case 12:
				pos2.y -= hJump;
				objxScale.x += 7.0f;
				objxScale.y += 1.5f;
				limitJump = 24;
				break;
			}
		}
		objX->transform->Scale(objxScale);
		transform->Position(pos2);
	}
}

void Player2::JumpShoot(int pos)
{
	JumpOrLay = 1;
	if (LeRiPos() == 1)
		offset = Vector3(20, 55, 0);
	if (LeRiPos() == 2)
		offset = Vector3(-20, 55, 0);
	if (LeRiPos() == 3)
		offset = Vector3(10, 55, 0);
	//슛페이크
	if (Input::Get()->KeyUp('E') && noStop == false)
	{
		if (LeRiPos() == 1 && posShoot == 1)//사이드
		{
			noColi = false;
			fake = true;
			anim->Play(L"flss");
		}
		else if (LeRiPos() == 2 && posShoot == 1)
		{
			noColi = false;
			fake = true;
			anim->Play(L"frss");
		}
		if (LeRiPos() == 1 && posShoot == 2)//대각
		{
			noColi = false;
			fake = true;
			anim->Play(L"flds");
		}
		else if (LeRiPos() == 2 && posShoot == 2)
		{
			noColi = false;
			fake = true;
			anim->Play(L"frds");
		}
		if (LeRiPos() == 3 && posShoot == 3)//중앙
		{
			noColi = false;
			fake = true;
			anim->Play(L"fcs");
		}
		noMove = 2;	//2는 피벗으로하잣
	}
	//피벗	----	애니메이션 따야됨^^
	/*if (Input::Get()->KeyDown('F') && noMove == 2)
	{

	}*/
	//슛
	if (Input::Get()->Key('E') || noStop == true)
	{
		objxFix = true;	// x충돌체 고정
		if (noMove == 0)	//슛하고 y20만큼뜨는문제제거
			transform->Position(pos2.x, pos2.y + 20);

		if (LeRiPos() == 1 && posShoot == 1) //사이드
		{
			transform->Scale(adjScale.x + 30.0f, adjScale.y + 40.0f);
			anim->Play(L"lss");
		}
		else if (LeRiPos() == 2 && posShoot == 1)
		{
			transform->Scale(adjScale.x + 30.0f, adjScale.y + 40.0f);
			anim->Play(L"rss");
		}
		if (LeRiPos() == 1 && posShoot == 2) //대각
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y + 30.0f);
			anim->Play(L"lds");
		}
		else if (LeRiPos() == 2 && posShoot == 2)
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y + 30.0f);
			anim->Play(L"rds");
		}
		if (LeRiPos() == 3 && posShoot == 3) //중앙
		{
			transform->Scale(adjScale.x + 25.0f, adjScale.y + 50.0f);
			anim->Play(L"cs");
		}
		UINT number = anim->getClip()->CurrentIndex();
		if (number >= 6 && number <= 8)
		{
			JumpControl(number);
			noStop = true;
		}
		else if (number >= 9 && number <= 11)
		{
			if (number == 9)
			{
				IsShoot(true);
			}
			JumpControl(number);
		}
		if (number >= 8 && number <= 11 && noMove == 1)
			blocking = true;
		else
			blocking = false;
	}
	//슛동작의 점프중일때 움직이지 않게
	if (LeRiPos() == 1)
	{
		if ((anim->IsPlay(L"lss") || anim->IsPlay(L"lds")) == true)
		{
			noMove = 1;
			noColi = true;
			fake = false;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"lss") || anim->IsPlay(L"lds"))
			== false && noStop == true)
		{
			handBall = false;
			noColi = false;
			noMove = 0;
			noStop = false;
			limitJump = 0;
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"standright");
			transform->Position(pos2.x, pos2.y - 20); //슛하고 y20만큼뜨는문제제거
		}
	}
	else if (LeRiPos() == 2)
	{
		if ((anim->IsPlay(L"rss") || anim->IsPlay(L"rds")) == true)
		{
			noMove = 1;
			noColi = true;
			fake = false;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"rss") || anim->IsPlay(L"rds"))
			== false && noStop == true)
		{
			handBall = false;
			noColi = false;
			noMove = 0;
			noStop = false;
			limitJump = 0;
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"standleft");
			transform->Position(pos2.x, pos2.y - 20); //슛하고 y20만큼뜨는문제제거
		}
	}
	else if (LeRiPos() == 3)
	{
		if (anim->IsPlay(L"cs") == true)
		{
			noMove = 1;
			noColi = true;
			fake = false;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if (anim->IsPlay(L"cs") == false && noStop == true)
		{
			handBall = false;
			noColi = false;
			noMove = 0;
			noStop = false;
			limitJump = 0;
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"standup");
			transform->Position(pos2.x, pos2.y - 20); //슛하고 y20만큼뜨는문제제거
		}
	}
}

void Player2::Block()
{
	if (Input::Get()->KeyDown('E') && block == false)
	{
		if (Input::Get()->Key('F')) //중앙
		{
			blockDirection = 1;
			objxFix = true;	// x충돌체 고정
			block = true;
			transform->Scale(adjScale.x + 30.0f, adjScale.y + 20.0f);
			anim->Play(L"cblock");
		}
		if (Input::Get()->Key('G') && Input::Get()->Key('F')) //오른쪽 대각선
		{
			blockDirection = 2;
			objxFix = true;	// x충돌체 고정
			block = true;
			transform->Scale(adjScale.x + 30.0f, adjScale.y + 20.0f);
			anim->Play(L"rdblock");
		}
		else if (Input::Get()->Key('G')) //오른쪽
		{
			blockDirection = 4;
			objxFix = true;	// x충돌체 고정
			block = true;
			transform->Scale(adjScale.x + 100.0f, adjScale.y + 40.0f);
			anim->Play(L"rblock");
		}
		if (Input::Get()->Key('D') && Input::Get()->Key('F')) //왼쪽 대각선
		{
			blockDirection = 3;
			objxFix = true;	// x충돌체 고정
			block = true;
			transform->Scale(adjScale.x + 30.0f, adjScale.y + 20.0f);
			anim->Play(L"ldblock");
		}
		else if (Input::Get()->Key('D')) //왼쪽
		{
			blockDirection = 5;
			objxFix = true;	// x충돌체 고정
			block = true;
			transform->Scale(adjScale.x + 100.0f, adjScale.y + 40.0f);
			anim->Play(L"lblock");
		}
	}
	if (block == true)
	{
		UINT number = anim->getClip()->CurrentIndex();
		if (number >= 0 && number <= 1)
		{
			JumpControl(number);
		}
		if (number >= 3 && number <= 8)
		{
			JumpControl(number);
		}
		else if (number >= 14 && number <= 19)
		{
			JumpControl(number);
		}
	}
	if (anim->IsPlay(L"cblock") == true)
	{
		block = true;
		noMove = 1;
	}
	//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
	else if (anim->IsPlay(L"cblock") == false
		&& block == true && blockDirection == 1)
	{
		block = false;
		limitJump = 0;
		blockDirection = 0;
		noMove = 0;
		transform->Scale(adjScale.x, adjScale.y);
		transform->Position(pos2.x - 20, pos2.y - 25); //블락하고 y20만큼뜨는문제제거
		__super::Update();
		anim->Play(L"standdown");
	}
	if (anim->IsPlay(L"rdblock") == true)
	{
		block = true;
		noMove = 1;
	}
	//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
	else if (anim->IsPlay(L"rdblock") == false
		&& block == true && blockDirection == 2)
	{
		block = false;
		limitJump = 0;
		blockDirection = 0;
		noMove = 0;
		transform->Scale(adjScale.x, adjScale.y);
		transform->Position(pos2.x - 20, pos2.y - 25); //블락하고 y20만큼뜨는문제제거
		__super::Update();
		anim->Play(L"standright");
	}
	if (anim->IsPlay(L"ldblock") == true)
	{
		block = true;
		noMove = 1;
	}
	//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
	else if (anim->IsPlay(L"ldblock") == false
		&& block == true && blockDirection == 3)
	{
		block = false;
		limitJump = 0;
		blockDirection = 0;
		noMove = 0;
		transform->Scale(adjScale.x, adjScale.y);
		transform->Position(pos2.x, pos2.y - 25); //블락하고 y20만큼뜨는문제제거
		__super::Update();
		anim->Play(L"standleft");
	}
	if (anim->IsPlay(L"rblock") == true)
	{
		block = true;
		noMove = 1;
	}
	//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
	else if (anim->IsPlay(L"rblock") == false
		&& block == true && blockDirection == 4)
	{
		block = false;
		limitJump = 0;
		blockDirection = 0;
		noMove = 0;
		transform->Scale(adjScale.x, adjScale.y);
		transform->Position(pos2.x - 40, pos2.y - 25); //블락하고 y20만큼뜨는문제제거
		__super::Update();
		anim->Play(L"standright");
	}
	if (anim->IsPlay(L"lblock") == true)
	{
		block = true;
		noMove = 1;
	}
	//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
	else if (anim->IsPlay(L"rblock") == false
		&& block == true && blockDirection == 5)
	{
		block = false;
		limitJump = 0;
		blockDirection = 0;
		noMove = 0;
		transform->Scale(adjScale.x, adjScale.y);
		transform->Position(pos2.x, pos2.y - 25); //블락하고 y20만큼뜨는문제제거
		__super::Update();
		anim->Play(L"standleft");
	}
}

void Player2::LayupDunk(int pos)
{
	JumpOrLay = 2;
	if (playmove == false)
		noChange[noChangein] = true;
	if (LeRiPos() == 1)
		offset = Vector3(20, 55, 0);
	if (LeRiPos() == 2)
		offset = Vector3(-20, 55, 0);
	if (LayOrDunk == 2)
	{
		if (LeRiPos() == 3)
			offset = Vector3(-20, 55, 0);
	}
	else
	{
		if (LeRiPos() == 3)
			offset = Vector3(10, 55, 0);
	}
	//레이업슛
	if (Input::Get()->KeyDown('E') || LayOrDunk == 1)
	{
		if (LeRiPos() == 1 && posShoot == 1 && Input::Get()->Key('G')) //사이드
		{
			if (noChange[0] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 45.0f, adjScale.y + 55.0f);
				anim->Play(L"lls");
			}
		}
		else if (LeRiPos() == 2 && posShoot == 1 && Input::Get()->Key('D'))
		{
			if (noChange[1] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 45.0f, adjScale.y + 55.0f);
				anim->Play(L"rls");
			}
		}
		if (LeRiPos() == 1 && posShoot == 2 && Input::Get()->Key('G')
			&& Input::Get()->Key('R')) //대각
		{
			if (noChange[2] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 45.0f, adjScale.y + 55.0f);
				anim->Play(L"lls");
			}
		}
		else if (LeRiPos() == 2 && posShoot == 2 && Input::Get()->Key('D')
			&& Input::Get()->Key('R'))
		{
			if (noChange[3] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 45.0f, adjScale.y + 55.0f);
				anim->Play(L"rls");
			}
		}
		if (LeRiPos() == 3 && posShoot == 3 && Input::Get()->Key('R')) //중앙
		{
			if (noChange[4] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 45.0f, adjScale.y + 55.0f);
				anim->Play(L"cls");
			}
		}
		if (fixi == true)
		{
			UINT number = anim->getClip()->CurrentIndex();
			if (number >= 7 && number <= 10)
				blocking = true;
			else
				blocking = false;
			if (number >= 0 && number <= 1)
			{
				LayOrDunk = 1;
				JumpControl(number);
			}
			if (number >= 2 && number <= 8)
			{
				JumpControl(number);
			}
			else if (number >= 9 && number <= 13)
			{
				if (number == 9)
				{
					IsShoot(true);
				}
				JumpControl(number);
			}
		}
	}
	//덩크슛
	if (Input::Get()->KeyDown('W') || LayOrDunk == 2)
	{
		if (LeRiPos() == 1 && posShoot == 1 && Input::Get()->Key('G')) //사이드
		{
			if (noChange[0] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 30.0f, adjScale.y + 40.0f);
				anim->Play(L"ldunk");
			}
		}
		else if (LeRiPos() == 2 && posShoot == 1 && Input::Get()->Key('D'))
		{
			if (noChange[1] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 30.0f, adjScale.y + 40.0f);
				anim->Play(L"rdunk");
			}
		}
		if (LeRiPos() == 1 && posShoot == 2 && Input::Get()->Key('G')
			&& Input::Get()->Key('R')) //대각
		{
			if (noChange[2] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 30.0f, adjScale.y + 40.0f);
				anim->Play(L"ldunk");
			}
		}
		else if (LeRiPos() == 2 && posShoot == 2 && Input::Get()->Key('D')
			&& Input::Get()->Key('R'))
		{
			if (noChange[3] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 30.0f, adjScale.y + 40.0f);
				anim->Play(L"rdunk");
			}
		}
		if (LeRiPos() == 3 && posShoot == 3 && Input::Get()->Key('R')) //중앙
		{
			if (noChange[4] == true)
			{
				objxFix = true;	// x충돌체 고정
				fixi = true;
				transform->Scale(adjScale.x + 65.0f, adjScale.y + 70.0f);
				anim->Play(L"mdunk");
			}
		}
		if (fixi == true)
		{
			UINT number = anim->getClip()->CurrentIndex();
			if (number >= 7 && number <= 10)
				blocking = true;
			else
				blocking = false;
			if (number >= 0 && number <= 1)
			{
				LayOrDunk = 2;
				JumpControl(number);
			}
			if (number >= 2 && number <= 8)
			{
				JumpControl(number);
			}
			else if (number >= 9 && number <= 13)
			{
				if (number == 9)
				{
					IsShoot(true);
				}
				JumpControl(number);
			}
		}
	}
	//슛동작의 점프중일때 움직이지 않게
	if (LeRiPos() == 1)
	{
		if ((anim->IsPlay(L"ldunk") || anim->IsPlay(L"lls")) == true)
		{
			noMove = 1;
			playmove = true;
			noColi = true;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"ldunk") || anim->IsPlay(L"lls"))
			== false && LayOrDunk != 0)
		{
			noChangeFalse();
			noColi = false;
			handBall = false;
			playmove = false;
			LayOrDunk = 0;
			noMove = 0;
			limitJump = 0;
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"standright");
			transform->Position(pos2.x, pos2.y - 20); //슛하고 y20만큼뜨는문제제거
		}
	}
	else if (LeRiPos() == 2)
	{
		if ((anim->IsPlay(L"rdunk") || anim->IsPlay(L"rls") == true))
		{
			noMove = 1;
			playmove = true;
			noColi = true;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"rdunk") || anim->IsPlay(L"rls"))
			== false && LayOrDunk != 0)
		{
			noChangeFalse();
			noColi = false;
			handBall = false;
			playmove = false;
			LayOrDunk = 0;
			noMove = 0;
			limitJump = 0;
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"standleft");
			transform->Position(pos2.x, pos2.y - 20); //슛하고 y20만큼뜨는문제제거
		}
	}
	else if (LeRiPos() == 3)
	{
		if ((anim->IsPlay(L"mdunk") || anim->IsPlay(L"cls")) == true)
		{
			noMove = 1;
			playmove = true;
			noColi = true;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"mdunk") || anim->IsPlay(L"cls")) == false && LayOrDunk != 0)
		{
			pos2.x -= 20.0f;
			noChangeFalse();
			noColi = false;
			handBall = false;
			playmove = false;
			LayOrDunk = 0;
			noMove = 0;
			limitJump = 0;
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"standup");
			transform->Position(pos2.x, pos2.y - 20); //슛하고 y20만큼뜨는문제제거
		}
	}
}

void Player2::SlideLayDunk()
{
	if (LeRiPos() == 1 && posShoot == 1)
	{
		pos2.x += 10.0f;
		objXPos.x += 10.0f;
		objX->transform->Position(objXPos);
	}
	else if (LeRiPos() == 2 && posShoot == 1)
	{
		pos2.x -= 10.0f;
		objXPos.x -= 10.0f;
		objX->transform->Position(objXPos);
	}
	else if (LeRiPos() == 1 && posShoot == 2)
	{
		pos2.x += 5.0f;
		pos2.y += 5.0f;
		objXPos.x += 5.0f;
		objXPos.y += 5.0f;
		objX->transform->Position(objXPos);
	}
	else if (LeRiPos() == 2 && posShoot == 2)
	{
		pos2.x -= 5.0f;
		pos2.y -= 5.0f;
		objXPos.x -= 5.0f;
		objXPos.y -= 5.0f;
		objX->transform->Position(objXPos);
	}
	else if (LeRiPos() == 3 && posShoot == 3)
	{
		pos2.y += 5.0f;
		objXPos.y += 5.0f;
		objX->transform->Position(objXPos);
	}
}

void Player2::Breakthrough()
{
	////돌파페이크
	//if (Input::Get()->KeyUp('I') && noStop == false)
	//{
	//	if (LeRiPos() == 1 && posShoot == 1)//사이드
	//		anim->Play(L"flss");
	//	else if (LeRiPos() == 2 && posShoot == 1)
	//		anim->Play(L"frss");
	//	if (LeRiPos() == 1 && posShoot == 2)//대각
	//		anim->Play(L"flds");
	//	else if (LeRiPos() == 2 && posShoot == 2)
	//		anim->Play(L"frds");
	//	if (LeRiPos() == 3 && posShoot == 3)//중앙
	//		anim->Play(L"fcs");
	//	noMove = 2;	//2는 피벗으로하잣
	//}
	//피벗	----	애니메이션 따야됨^^
	/*if (Input::Get()->KeyDown('F') && noMove == 2)
	{

	}*/
	//돌파
	if (Input::Get()->KeyDown('Q') && breaker == false)
	{
		if (LeRiPos() == 1 && posShoot == 1) //사이드
		{
			if (Input::Get()->Key('R'))
			{
				transform->Scale(adjScale.x + 55.0f, adjScale.y + 10.0f);
				anim->Play(L"lslbt");
				if (breakth != 10)
					breakth = 1;
			}
			if (Input::Get()->Key('F'))
			{
				transform->Scale(adjScale.x + 55.0f, adjScale.y + 10.0f);
				anim->Play(L"lsrbt");
				if (breakth != 10)
					breakth = 2;
			}
		}
		else if (LeRiPos() == 2 && posShoot == 1)
		{
			if (Input::Get()->Key('R'))
			{
				transform->Scale(adjScale.x + 55.0f, adjScale.y + 10.0f);
				anim->Play(L"rsrbt");
				if (breakth != 10)
					breakth = 3;
			}
			if (Input::Get()->Key('F'))
			{
				transform->Scale(adjScale.x + 55.0f, adjScale.y + 10.0f);
				anim->Play(L"rslbt");
				if (breakth != 10)
					breakth = 4;
			}
		}
		if (LeRiPos() == 1 && posShoot == 2) //대각
		{
			if (Input::Get()->Key('D') || Input::Get()->Key('R'))
			{
				transform->Scale(adjScale.x + 50.0f, adjScale.y + 10.0f);
				anim->Play(L"ldlbt");
				if (breakth != 10)
					breakth = 5;
			}
			if (Input::Get()->Key('G'))
			{
				transform->Scale(adjScale.x + 90.0f, adjScale.y + 15.0f);
				anim->Play(L"ldrbt");
				if (breakth != 10)
					breakth = 6;
			}
		}
		else if (LeRiPos() == 2 && posShoot == 2)
		{
			if (Input::Get()->Key('D'))
			{
				transform->Scale(adjScale.x + 90.0f, adjScale.y + 15.0f);
				anim->Play(L"rdlbt");
				if (breakth != 10)
					breakth = 7;
			}
			if (Input::Get()->Key('G') || Input::Get()->Key('R'))
			{
				transform->Scale(adjScale.x + 50.0f, adjScale.y + 10.0f);
				anim->Play(L"rdrbt");
				if (breakth != 10)
					breakth = 8;
			}
		}
		if (LeRiPos() == 3 && posShoot == 3) //중앙
		{
			if (Input::Get()->Key('D'))
			{
				transform->Scale(adjScale.x + 70.0f, adjScale.y + 15.0f);
				anim->Play(L"clbt");
				if (breakth != 10)
					breakth = 9;
			}
			if (Input::Get()->Key('G'))
			{
				transform->Scale(adjScale.x + 70.0f, adjScale.y + 15.0f);
				anim->Play(L"crbt");
				if (breakth != 10)
					breakth = 11;
			}
		}
	}
	//=================돌파의 방향추가==================switch (breakth)
	switch (breakth)
	{
	case 1:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x += 200.0f;
		breakpos.y += 30.0f;
		breakthSpeed = 3.0f;
		breakth = 10;
		break;
	case 2:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x += 200.0f;
		breakpos.y -= 30.0f;
		breakthSpeed = 3.0f;
		breakth = 10;
		break;
	case 3:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x -= 200.0f;
		breakpos.y += 30.0f;
		breakthSpeed = 3.0f;
		breakth = 10;
		break;
	case 4:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x -= 200.0f;
		breakpos.y -= 30.0f;
		breakthSpeed = 3.0f;
		breakth = 10;
		break;
	case 5:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x += 60.0f;
		breakpos.y += 130.0f;
		breakthSpeed = 2.0f;
		breakth = 10;
		break;
	case 6:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x += 210.0f;
		breakpos.y += 60.0f;
		breakthSpeed = 3.0f;
		breakth = 10;
		break;
	case 7:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x -= 210.0f;
		breakpos.y += 60.0f;
		breakthSpeed = 3.0f;
		breakth = 10;
		break;
	case 8:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x -= 60.0f;
		breakpos.y += 130.0f;
		breakthSpeed = 2.0f;
		breakth = 10;
		break;
	case 9:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x -= 120.0f;
		breakpos.y += 120.0f;
		breakthSpeed = 2.0f;
		breakth = 10;
		break;
	case 11:
		breakth2 = breakth;
		breakpos = pos2;
		breakpos.x += 120.0f;
		breakpos.y += 120.0f;
		breakthSpeed = 2.0f;
		breakth = 10;
		break;
	default:
		break;
	}
	UINT number = anim->getClip()->CurrentIndex();	// 세번째동작부터 돌파하기 시작
	if (breakth == 10 && breaker == true && number > 2)
	{
		Vector3 livepos = transform->Position();
		Vector3 temp = breakpos - livepos;
		D3DXVec3Normalize(&temp, &temp);
		temp.x += g_ETime;
		temp.y += g_ETime;
		//돌파 스피드 결정 switch (breakth2)
		switch (breakth2)
		{
		case 1:
			temp.y += 1.0f;
			temp.x += breakthSpeed;
			break;
		case 2:
			temp.y -= 1.0f;
			temp.x += breakthSpeed;
			break;
		case 3:
			temp.y += 1.0f;
			temp.x -= breakthSpeed;
			break;
		case 4:
			temp.y -= 1.0f;
			temp.x -= breakthSpeed;
			break;
		case 5:
			temp.y += breakthSpeed;
			temp.x += breakthSpeed;
			break;
		case 6:
			temp.y += 0.5f;
			temp.x += breakthSpeed;
			break;
		case 7:
			temp.y += 0.5f;
			temp.x -= breakthSpeed;
			break;
		case 8:
			temp.y += breakthSpeed;
			temp.x -= breakthSpeed;
			break;
		case 9:
			temp.x -= breakthSpeed;
			temp.y += breakthSpeed;
			break;
		case 11:
			temp.x += breakthSpeed;
			temp.y += breakthSpeed;
			break;

		}
		pos2 += temp;	//적용된 돌파속도를 위치에 적용
		//돌파 범위 제한 switch (breakth2)
		switch (breakth2)
		{
		case 1:
			if (livepos.x < breakpos.x)
				transform->Position(pos2);
			break;
		case 2:
			if (livepos.x < breakpos.x)
				transform->Position(pos2);
			break;
		case 3:
			if (livepos.x > breakpos.x)
				transform->Position(pos2);
			break;
		case 4:
			if (livepos.x > breakpos.x)
				transform->Position(pos2);
			break;
		case 5:
			if (livepos.y < breakpos.y)
				transform->Position(pos2);
			break;
		case 6:
			if (livepos.x < breakpos.x)
				transform->Position(pos2);
			break;
		case 7:
			if (livepos.x > breakpos.x)
				transform->Position(pos2);
			break;
		case 8:
			if (livepos.y < breakpos.y)
				transform->Position(pos2);
			break;
		case 9:
			if (livepos.x > breakpos.x)
				transform->Position(pos2);
			break;
		case 11:
			if (livepos.x < breakpos.x)
				transform->Position(pos2);
			break;
		}
	}
	//슛동작의 점프중일때 움직이지 않게
	if (LeRiPos() == 1)
	{
		if ((anim->IsPlay(L"lslbt") || anim->IsPlay(L"lsrbt") ||
			anim->IsPlay(L"ldlbt") || anim->IsPlay(L"ldrbt")) == true)
		{
			breaker = true;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"lslbt") && anim->IsPlay(L"lsrbt") &&
			anim->IsPlay(L"ldlbt") && anim->IsPlay(L"ldrbt")) == false && breaker == true)
		{
			breaker = false;
			breakth = 0;
			anim->Stop();
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"bmr");
		}
	}
	else if (LeRiPos() == 2)
	{
		if ((anim->IsPlay(L"rsrbt") || anim->IsPlay(L"rslbt") ||
			anim->IsPlay(L"rdrbt") || anim->IsPlay(L"rdlbt")) == true)
		{
			breaker = true;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"rsrbt") && anim->IsPlay(L"rslbt") &&
			anim->IsPlay(L"rdrbt") && anim->IsPlay(L"rdlbt")) == false && breaker == true)
		{
			breaker = false;
			breakth = 0;
			anim->Stop();
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"bml");
		}
	}
	else if (LeRiPos() == 3)
	{
		if ((anim->IsPlay(L"clbt") || anim->IsPlay(L"crbt")) == true)
		{
			breaker = true;
		}
		//슛이 끝나고나서 움직일수 있고 공의 소유를 없앰
		else if ((anim->IsPlay(L"clbt") && anim->IsPlay(L"crbt")) == false && breaker == true)
		{
			breaker = false;
			breakth = 0;
			anim->Stop();
			transform->Scale(adjScale.x, adjScale.y);
			__super::Update();
			anim->Play(L"bmu");
		}
	}
}

void Player2::Stopbreakth()
{
	anim->Stop();
}

Vector3 Player2::Shoot()
{
	Vector3 in = pos2 + offset;
	return Vector3(in);
}

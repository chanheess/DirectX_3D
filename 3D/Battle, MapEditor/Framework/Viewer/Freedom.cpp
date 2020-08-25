#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
	: Camera()
{

}

Freedom::~Freedom()
{

}

void Freedom::Update()
{
	if (Keyboard::Get()->Down(VK_F8))
	{
		if (changeLookAt == false)
			changeLookAt = true;
		else if (changeLookAt == true)
			changeLookAt = false;
	}

	Pos();
	Rot();
}

void Freedom::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}

void Freedom::Pos()
{
	Vector3 f = Forward();
	Vector3 u = Up();
	Vector3 r = Right();

	//Move
	if (Mouse::Get()->Press(1) && changeLookAt == false)
	{
		Vector3 P;
		Position(&P);

		if (Keyboard::Get()->Press('W'))
			P = P + f * move * Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			P = P - f * move * Time::Delta();

		if (Keyboard::Get()->Press('D'))
			P = P + r * move * Time::Delta();
		else if (Keyboard::Get()->Press('A'))
			P = P - r * move * Time::Delta();

		if (Keyboard::Get()->Press('E'))
			P = P + u * move * Time::Delta();
		else if (Keyboard::Get()->Press('Q'))
			P = P - u * move * Time::Delta();

		Position(P);
	}
	else if (changeLookAt == true)
	{
		Vector3 P;
		Position(&P);

		Vector3 R;
		Rotation(&R);

		//follow
		if (lookStyle == false)
		{
			P.x = sin(R.y) * cos(R.x) * distance;
			P.y = sin(-R.x) * (distance + 30.0f);
			P.z = cos(R.y) * cos(R.x) * distance;
		}
		//topdown
		else if (lookStyle == true)
		{
			P.x = sin(R.y) * cos(R.x) * distance;
			P.y = sin(-R.x) * (distance + 40.0f);
			P.z = cos(R.y) * cos(R.x) * distance + 10.0f;

			R.x = 0.646373f;
		}

		Position(at - P);

		Rotation(R);
	}
}

void Freedom::Rot()
{
	//Rotation
	if (Mouse::Get()->Press(1) && changeLookAt == false)
	{
		Vector3 R;
		Rotation(&R);

		Vector3 val = Mouse::Get()->GetMoveValue();
		
		R.x = R.x + val.y * rotation * Time::Delta();
		R.y = R.y + val.x * rotation * Time::Delta();
		R.z = 0.0f;

		Rotation(R);
	}
	//followView
	else if (changeLookAt == true)
	{
		Vector3 val = Mouse::Get()->GetMoveValue();

		Vector3 RD;
		RotationDegree(&RD);

		if (lookStyle == false)
		{
			RD.x = RD.x + val.y * rotation * 0.2f;

			if (RD.x >= 50.0f)
				RD.x = 50.0f;
			else if (RD.x <= 3.0f)
				RD.x = 3.0f;

			RD.y = RD.y + val.x * rotation * 0.2f;

			//D3DDesc desc = D3D::GetDesc();
			//UINT centerX = ((UINT)desc.Width) / 2;
			//UINT centerY = ((UINT)desc.Height) / 2;
			//ClientToScreen(g_hWnd, val);
			//SetCursorPos(centerX, centerX);
			//ShowCursor(false);
		}
		RD.z = 0.0f;

		RotationDegree(RD);
	}
}

void Freedom::LookAt(Vector3 pos, float scaleY)
{
	at = Vector3(pos.x, pos.y + scaleY, pos.z);
}

void Freedom::BattleViewStyle(bool style)
{
	if (style == false)	//3ÀÎÄª
	{
		if (changeLookAt == false)
		{
			changeLookAt = true;
			lookStyle = false;
		}
	}
	else	//topdown
	{
		if (changeLookAt == false)
		{
			changeLookAt = true;
			lookStyle = true;
		}
	}
}

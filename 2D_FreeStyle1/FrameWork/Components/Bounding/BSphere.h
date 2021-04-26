#pragma once
//fileName : BSphere.h
#include "stdafx.h"
struct BSphere : public ILine
{
	BSphere() : Center(0,0,0), radius(50), count(30)
	{

	}

	BSphere Transform(D3DXMATRIX world)
	{
		BSphere temp;
		temp.Center = Center;
		temp.radius = radius;

		D3DXVECTOR3 right = D3DXVECTOR3(1,0,0);
		D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

		D3DXVec3TransformNormal(&right, &right, &world);
		D3DXVec3TransformNormal(&up, &up, &world);

		D3DXVECTOR3 scale;
		scale.x = D3DXVec3Length(&right);
		scale.y = D3DXVec3Length(&up);
		//Coord -> 좌표
		//Normal-> 축 (회전, 크기)
		
		if (scale.x > scale.y)
			temp.radius *= scale.x;
		else
			temp.radius *= scale.y;

		D3DXVec3TransformCoord(&temp.Center,
			&temp.Center, &world);

		return temp;
	}
	void GetLine(vector<Vertex>* vertices, D3DXMATRIX world) override
	{
		vector<Vertex> temp;
		BSphere sphere = Transform(world);
		//360 / 30 -> 12 *
		//i < 30 (29 + 1) -> 30  12 * 30 360 
		float phiStep = 2 * D3DX_PI / count;
		for (int i = 0; i < count; i++)
		{
			temp.push_back
			(
				Vertex
				(
					sphere.Center.x + cos(i * phiStep) *sphere.radius, 
					sphere.Center.y + sin(i * phiStep) *sphere.radius
				)
			);
			temp.push_back
			(
				Vertex
				(
					sphere.Center.x + cos((i + 1) * phiStep) *sphere.radius,
					sphere.Center.y + sin((i + 1)* phiStep) * sphere.radius
				)
			);
		}
		*vertices = temp;
	}
	//2가지
	D3DXVECTOR3 Center; //중심점
	float radius; //반지름 

	UINT count; //선의 갯수
};
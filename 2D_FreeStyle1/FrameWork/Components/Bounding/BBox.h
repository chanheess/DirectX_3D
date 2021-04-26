#pragma once
#include "stdafx.h"
#include "./Components/Component.h"
struct BBox : public ILine, public Component
{

	BBox() : Component(L"BoxCollider")
	{
		Min = D3DXVECTOR3(-0.5f, -0.5f, 0);
		Max = D3DXVECTOR3(0.5f, 0.5f, 0);
	}
	BBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
		: Component(L"BoxCollider"),
		Min(min), Max(max){}

	void Insfactor() override
	{

	}
	BBox Transform(D3DXMATRIX world) 
	{
		BBox temp;
		temp.Vec[0] = { Min.x, Min.y,0.0f };
		temp.Vec[1] = { Min.x, Max.y,0.0f };
		temp.Vec[2] = { Max.x, Max.y,0.0f };
		temp.Vec[3] = { Max.x, Min.y,0.0f };

		for (int i = 0; i < 4; i++)
			D3DXVec3TransformCoord(temp.Vec + i, temp.Vec + i, &world);

		temp.Min = temp.Vec[0];
		temp.Max = temp.Vec[0];

		for (int i = 1; i < 4; i++)
		{
			if (temp.Min.x > temp.Vec[i].x)
				temp.Min.x = temp.Vec[i].x;
			if (temp.Max.x < temp.Vec[i].x)
				temp.Max.x = temp.Vec[i].x;

			if (temp.Min.y > temp.Vec[i].y)
				temp.Min.y = temp.Vec[i].y;
			if (temp.Max.y < temp.Vec[i].y)
				temp.Max.y = temp.Vec[i].y;
		}
		return temp;
	}
	virtual void GetLine(vector<Vertex>* vertices, D3DXMATRIX world) override
	{
		BBox box = Transform(world);
		vector<Vertex> temp;

		//OBB -> 회전한 물체
		for (int i = 0; i < 4; i++)
		{
			temp.push_back(Vertex(box.Vec[i]));
			temp.push_back(Vertex(box.Vec[(i + 1) % 4]));
		}

		//AABB -> 회전하지 않는 물체 
		//left
		temp.push_back(Vertex(box.Min.x, box.Min.y));
		temp.push_back(Vertex(box.Min.x, box.Max.y));

		//top
		temp.push_back(Vertex(box.Min.x, box.Max.y));
		temp.push_back(Vertex(box.Max.x, box.Max.y));

		//right
		temp.push_back(Vertex(box.Max.x, box.Max.y));
		temp.push_back(Vertex(box.Max.x, box.Min.y));

		//bottom;
		temp.push_back(Vertex(box.Max.x, box.Min.y));
		temp.push_back(Vertex(box.Min.x, box.Min.y));

		*vertices = temp;
	}

	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;

	D3DXVECTOR3 Vec[4];
};
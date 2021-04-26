#include "stdafx.h"
#include "Intersect.h"
#include "./Components/Bounding/BBox.h"
#include "./Components/Bounding/BSphere.h"
#include "./Components/Objects/Transform.h"

#include "./GameObjects/GameObject.h"

bool Intersect::IsConstantsPoint(D3DXMATRIX W, D3DXVECTOR3 vec)
{
	BBox temp;
	temp = temp.Transform(W);
	bool bCheck = true;
	bCheck &= temp.Min.x < vec.x;
	bCheck &= temp.Max.x > vec.x;
	bCheck &= temp.Min.y < vec.y;
	bCheck &= temp.Max.y > vec.y;

	return bCheck;
}

bool Intersect::IsConstantsPoint(BBox box, D3DXMATRIX W, D3DXVECTOR3 vec)
{
	box = box.Transform(W);
	bool bCheck = true;
	bCheck &= box.Min.x < vec.x;
	bCheck &= box.Max.x > vec.x;
	bCheck &= box.Min.y < vec.y;
	bCheck &= box.Max.y > vec.y;

	return bCheck;
}

bool Intersect::IsConstantsPoint(GameObject * obj, D3DXVECTOR3 point)
{
	BBox* box = obj->GetComponent<BBox>();
	if (box != NULL)
	{
		return IsConstantsPoint(*box, obj->transform->World(), point);
	}
	return false;
}

//1-----2		//1---2    3
//|		|		//|  /	  /|
//|		|		//| /	 / |
//0-----3		//0		3--5

bool Intersect::IsConstantsPoint(vector<D3DXVECTOR3> verticse, D3DXVECTOR3 vec)
{
	D3DXVECTOR3 p0, p1, p2;
	p0 = verticse[0];
	p1 = verticse[1];
	p2 = verticse[2];
	bool bCheck = false;
	bCheck |= D3DXIntersectTri
	(
		&p0, &p1, &p2,
		&vec, &(vec + D3DXVECTOR3(0, 0, 1)),
		0, 0, 0
	);
	if (verticse.size() == 4)
	{
		p1 = verticse[2];
		p2 = verticse[3];
	}
	else if (verticse.size() == 6)
	{
		p0 = verticse[3];
		p1 = verticse[4];
		p2 = verticse[5];
	}
	bCheck |= D3DXIntersectTri
	(
		&p0, &p1, &p2,
		&vec, &(vec + D3DXVECTOR3(0, 0, 1)),
		0, 0, 0
	);
	return bCheck;
}

bool Intersect::IsConstantsPointCircle(D3DXMATRIX world, D3DXVECTOR3 vec)
{
	BSphere sphere;
	sphere = sphere.Transform(world);

	float length = D3DXVec3Length(&(sphere.Center - vec));
	return length < sphere.radius;
}

bool Intersect::IsConstantsRect(BBox* temp, D3DXMATRIX comp1, D3DXMATRIX comp2)
{
	BBox boxA;
	boxA = boxA.Transform(comp1);

	BBox boxB;
	boxB = boxB.Transform(comp2);

	bool Horizontal =false;
	bool Vertical = false;

	if (boxA.Min.x < boxB.Max.x && boxA.Max.x > boxB.Min.x)
	{
		Horizontal = true;
	}
	if (boxA.Min.y < boxB.Max.y && boxA.Max.y > boxB.Min.y)
	{
		Vertical = true;
	}
	if (Horizontal && Vertical)
	{
		if (temp != NULL)
		{
			temp->Min.x = (boxA.Min.x > boxB.Min.x) ? boxA.Min.x : boxB.Min.x;
			temp->Max.x = (boxA.Max.x < boxB.Max.x) ? boxA.Max.x : boxB.Max.x;
			temp->Min.y = (boxA.Min.y > boxB.Min.y) ? boxA.Min.y : boxB.Min.y;
			temp->Max.y = (boxA.Max.y < boxB.Max.y) ? boxA.Max.y : boxB.Max.y;

		}
		return true;
	}
	return false;
}

bool Intersect::IsConstantsRectCircle(D3DXMATRIX box, D3DXMATRIX Sphere)
{
	BBox boxA;
	boxA = boxA.Transform(box);
	BSphere sphereA;
	sphereA = sphereA.Transform(Sphere);

	if ((boxA.Min.x < sphereA.Center.x && sphereA.Center.x < boxA.Max.x) ||
		(boxA.Min.y < sphereA.Center.y&& sphereA.Center.y < boxA.Max.y))
	{
		BBox boxB;
		boxB.Max.x += sphereA.radius;
		boxB.Max.y += sphereA.radius;
		boxB.Min.x -= sphereA.radius;
		boxB.Min.y -= sphereA.radius;

		return IsConstantsPoint(boxB, box, sphereA.Center);
	}
	else
	{
		bool bCheck = false;

		bCheck |= IsConstantsPointCircle(Sphere, boxA.Min);
		bCheck |= IsConstantsPointCircle(Sphere, boxA.Max);
		bCheck |= IsConstantsPointCircle(Sphere, 
			D3DXVECTOR3(boxA.Min.x, boxA.Max.y, 0.0f));
		bCheck |= IsConstantsPointCircle(Sphere,
			D3DXVECTOR3(boxA.Max.x, boxA.Min.y, 0.0f));

		return bCheck;
	}
}

bool Intersect::IsConstantsSphere(D3DXMATRIX world1, D3DXMATRIX world2)
{
	BSphere sphereA;
	sphereA = sphereA.Transform(world1);//

	D3DXVECTOR3 centerA = sphereA.Center;
	float radiusA = sphereA.radius;
	BSphere sphereB;
	sphereB = sphereB.Transform(world2);//

	D3DXVECTOR3 centerB = sphereB.Center;
	float radiusB = sphereB.radius;


	return D3DXVec3Length(&(centerA - centerB)) < radiusA + radiusB;
}

bool Intersect::IsConstantsObject(GameObject * obj1, GameObject * obj2)
{
	BBox* boxA = obj1->GetComponent<BBox>();
	BBox* boxB = obj2->GetComponent<BBox>();

	if (boxA == NULL || boxB == NULL) return false;

	boxA = &boxA->Transform(obj1->transform->World());
	boxB = &boxB->Transform(obj2->transform->World());


	bool Horizontal = false;
	bool Vertical = false;

	if (boxA->Min.x < boxB->Max.x && boxA->Max.x > boxB->Min.x)
	{
		Horizontal = true;
	}
	if (boxA->Min.y < boxB->Max.y && boxA->Max.y > boxB->Min.y)
	{
		Vertical = true;
	}
	if (Horizontal && Vertical)
	{
		return true;
	}
	return false;
}

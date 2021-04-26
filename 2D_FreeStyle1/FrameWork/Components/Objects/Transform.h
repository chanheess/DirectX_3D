#pragma once
#include "./Components/Component.h"
//----------------------------------------------------------------
//	Transform(이동, 회전, 크기)
//----------------------------------------------------------------
enum class Space { Local = 0, World };


__declspec(align(16))
class Transform : public Component
{
private:
	friend class GameObject;
public:
	Transform();
	~Transform();

	void Update() override;
	void Render() override;
	void Insfactor() override;

public:
	void Pivot(D3DXVECTOR2 vec);

	void Position(float x, float y);
	void Position(D3DXVECTOR3 vec);
	D3DXVECTOR3 Position(Space s = Space::Local);

	void Scale(float x, float y);
	void Scale(D3DXVECTOR3 vec);
	D3DXVECTOR3 Scale(Space s = Space::Local);

	void Rotation(float radian);
	float Rotation() { return angle; }

	void RotationDegree(float degree);
	float RotationDegree() { return Math::ToDegree(angle); }

	D3DXMATRIX World() { return world; }
public:
	void AddChild(class GameObject* newChild);
	void AttachTo(class GameObject* newParent);

	void RelaseParent();

private:
	D3DXVECTOR3 position; //위치
	D3DXVECTOR3 scale; //크기 
	float angle; //각도
	D3DXVECTOR2 pivot;

	D3DXMATRIX world; //충돌
private:
	struct CB_World
	{
		D3DXMATRIX data;
	}worldDesc;
	class CBuffer * worldBuffer;

private:
	Transform * pParent;
	Transform * pFirstChild;
	Transform * pNextSibling;	//??
};


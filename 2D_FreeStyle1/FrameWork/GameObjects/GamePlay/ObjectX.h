#pragma once
#include "./GameObjects/GameObject.h"

class ObjectX : public GameObject
{
public:
	ObjectX();
	~ObjectX();

	void Update() override;
	void Render() override;

	void Pos(Vector3 pos, int option);
	
private:
	Vector3 pos2;
};


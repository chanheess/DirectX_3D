#pragma once
#include "./GameObjects/GameObject.h"

class Goaldae2 : public GameObject
{
public:
	Goaldae2();
	~Goaldae2();

	void Update() override;
	void Render() override;
};


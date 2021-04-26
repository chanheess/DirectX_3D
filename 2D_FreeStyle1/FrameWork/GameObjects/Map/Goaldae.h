#pragma once
#include "./GameObjects/GameObject.h"

class Goaldae : public GameObject
{
public:
	Goaldae();
	~Goaldae();

	void Update() override;
	void Render() override;
};


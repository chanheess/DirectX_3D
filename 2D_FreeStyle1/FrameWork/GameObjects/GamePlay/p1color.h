#pragma once
#include "./GameObjects/GameObject.h"

class p1color : public GameObject
{
public:
	p1color();
	~p1color();

	void Update() override;
	void Render() override;

};
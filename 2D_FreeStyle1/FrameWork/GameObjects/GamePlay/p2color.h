#pragma once
#include "./GameObjects/GameObject.h"

class p2color : public GameObject
{
public:
	p2color();
	~p2color();

	void Update() override;
	void Render() override;

};
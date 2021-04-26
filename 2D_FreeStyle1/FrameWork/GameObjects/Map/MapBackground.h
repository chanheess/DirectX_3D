#pragma once
#include "./GameObjects/GameObject.h"

class MapBackground : public GameObject
{
public:
	MapBackground();
	~MapBackground();

	void Update() override;
	void Render() override;
};

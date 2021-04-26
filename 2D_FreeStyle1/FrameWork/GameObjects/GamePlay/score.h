#pragma once
#include "./GameObjects/GameObject.h"

class score : public GameObject, public CMemeoryPool<score>
{
public:
	score();
	~score();

	void Update() override;
	void Render() override;
};


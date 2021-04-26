#pragma once
#include "./GameObjects/GameObject.h"
class Canvers : public GameObject
{
public:
	Canvers();
	~Canvers();

	void Ready() override;
	void Update() override;
	void Render() override;
private:
	struct Struct
	{
		Matrix View;
		Matrix Ortho;
	};
	class CBuffer* buffer;
};


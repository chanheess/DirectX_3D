#pragma once
#include "./Systems/Gui.h"
#include "./GameObjects/GameObject.h"
class ProgressBar : public GameObject
{
public:
	ProgressBar(wstring name = L"ProgressBar");
	~ProgressBar();

	void Update();
	void Render();

	void SetGauge(float gauge);
private:
	class GameObject* back;  //뒷면 이미지
	class GameObject* front; //앞면 이미지

	float nowGauge;
	float totalGauge;
};


#pragma once
#include "GameObjects/GameObject.h"
class Button : public GameObject
{
public:
	Button();
	~Button();
	
	void Ready() override;
	void Update() override;
	void Render() override;
	
	void Insfactor() override;

	void AddOneClickEvent(function<void(void)> fp);
private:
	class Sprite* after;

	vector<function<void(void)>> events;
	bool bClick;
};


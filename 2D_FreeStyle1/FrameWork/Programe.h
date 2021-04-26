#pragma once
#include "./Systems/Window.h"

class Programe : public Window
{
public:
	Programe();
	~Programe();

	// Window을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;

private:
	class Audio* audio;
};


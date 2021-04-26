#pragma once
#include "./Components/Component.h"
class Animator : public Component
{
public:
	Animator();
	~Animator();

	void Update();
	void Render();

	void Play(wstring name);
	void Stop();

	class Clip* getClip() { return currentClip; }

	bool IsPlay(wstring name);
	bool IsPlay();

	void AddClip(class Clip* clip);

	void Insfactor() override;

	class Sprite* GetSprite();
private:
	vector<class Clip*> clips;
	class Clip* currentClip;
};


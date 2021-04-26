#pragma once

#include "Components/Component.h"
#define CHANNELBUFFER 5
#define SOUNDBUFFER 20

#define TOTALBUFFER (CHANNELBUFFER + SOUNDBUFFER)

class Audio : public Component, public CMemeoryPool<Audio>
{
public:
	static void Create();
	static void Delete();

public:
	Audio();
	~Audio();

	void Set(wstring file,
		bool bgm = false, bool loop = false);

	void Update();

	//0.0f ~ 1.0f
	void Play(int volum = 1.0f);
	void Stop();
	void Pause();
	void Resume();

	bool isPlaySound();
	bool isPauseSound();

private:
	static FMOD::System* system;

	FMOD::Sound* sound;
	FMOD::Channel* channel;
};


#include "stdafx.h"
#include "Audio.h"
#include "Utilities/String.h"
#include "Utilities/Path.h"
using namespace FMOD;
System* Audio::system = NULL;

void Audio::Create()
{
	if (system != NULL) return;

	System_Create(&system);
	system->init(TOTALBUFFER, FMOD_INIT_NORMAL, 0);
}

void Audio::Delete()
{
	if (system != NULL)
	{
		system->release();
		system->close();
	}
}

Audio::Audio()
	: Component(L"AudioSource"), sound(NULL), channel(NULL)
{
	
}

Audio::~Audio()
{
	if (sound != NULL)
	{
		sound->release();
		sound = NULL;
	}
	if (channel != NULL)
	{
		channel->stop();
		channel = NULL;
	}
}

void Audio::Set(wstring file, bool bgm, bool loop)
{
	if (sound != NULL)
	{	
		sound->release(); sound = NULL;
	}
	if (channel != NULL)
	{
		channel->stop(); channel = NULL;
	}


	string fileName = "../_Resources/Sounds/" + String::ToString(file);
	assert(Path::ExistFile(fileName));

	if (loop)
	{
		if (bgm)
		{
			system->createStream
			(
				fileName.c_str(),
				FMOD_LOOP_NORMAL,
				NULL, &sound
			);
		}
		else
		{
			system->createSound
			(
				fileName.c_str(),
				FMOD_LOOP_NORMAL,
				NULL, &sound
			);
		}
	}
	else
	{
		system->createSound
		(
			fileName.c_str(),
			FMOD_DEFAULT,
			NULL, &sound
		);
	}

}

void Audio::Update()
{
	if(system != NULL)
		system->update();
}

void Audio::Play(int volum)
{
	if (sound == NULL) return;

	system->playSound
	(
		FMOD_CHANNEL_FREE, sound, false, &channel
	);
	channel->setVolume(volum);
}

void Audio::Stop()
{
	if (sound == NULL) return;

	channel->stop();
}

void Audio::Pause()
{
	if (sound == NULL) return;
	channel->setPaused(true);
}

void Audio::Resume()
{
	if (sound == NULL) return;
	channel->setPaused(false);
}

bool Audio::isPlaySound()
{
	if (sound == NULL) return false;
	bool b = false;
	channel->isPlaying(&b);
	return b;
}

bool Audio::isPauseSound()
{
	if (sound == NULL) return false;
	bool b = false;
	channel->getPaused(&b);
	return b;
}

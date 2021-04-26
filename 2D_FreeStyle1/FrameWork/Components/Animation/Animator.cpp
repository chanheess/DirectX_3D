#include "stdafx.h"
#include "Animator.h"
#include "./Clip.h"
#include "./Sprite.h"
#include "./GameObjects/GameObject.h"
#include "./Components/Objects/Transform.h"
Animator::Animator()
	:currentClip(NULL), Component(L"Animator")
{
}


Animator::~Animator()
{
	for (int i = 0; i < clips.size(); i++)
		SAFE_DELETE(clips[i]);
}

void Animator::Update()
{
	if (currentClip != NULL)
	{
		currentClip->Update();
		
	}
}

void Animator::Render()
{
	if (currentClip != NULL)
		currentClip->Render();
}

void Animator::Play(wstring name)
{
	for (Clip* clip : clips)
	{
		if (clip->GetName() == name)
		{
			if(currentClip == NULL)
				currentClip = clip;
			else
			{
				if (currentClip == clip) return;

				currentClip->Stop();
				currentClip = clip;
			}
			currentClip->Play();
			break;
		}
	}
}

void Animator::Stop()
{
	if (currentClip != NULL)
		currentClip->Stop();
}

bool Animator::IsPlay(wstring name)
{
	for (Clip* clip : clips)
	{
		if (clip->GetName() == name)
			return clip->IsPlay();
	}
	return false;
}

bool Animator::IsPlay()
{
	return currentClip->IsPlay();
}

void Animator::AddClip(Clip * clip)
{
	clips.push_back(clip);
}

void Animator::Insfactor()
{
	__super::Insfactor();
	if (ImGui::Button("Play"))
	{
		if (currentClip != NULL)
			Play(currentClip->GetName());
	}
	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		if (currentClip != NULL)
			Stop();
	}
}

Sprite * Animator::GetSprite()
{
	return currentClip->CurrentSprite();
}

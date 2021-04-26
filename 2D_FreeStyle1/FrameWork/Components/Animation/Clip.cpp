#include "stdafx.h"
#include "Clip.h"
#include "./Sprite.h"
#include "./Utilities/String.h"
Clip::Clip(wstring name, AnimationType type)
	:name(name), type(type)
{
	bPlay = false; //Ãß°¡ 
	currentFrame = 0;
}

Clip::~Clip()
{
	for (int i = 0; i < frames.size(); i++)
		SAFE_DELETE(frames[i].Image);
}

void Clip::Update()
{
	if (bPlay == false) return;

	totalTime += g_ETime;
	if (totalTime > frames[currentFrame].Time)
	{
		totalTime -= frames[currentFrame].Time;
		switch (type)
		{
			case AnimationType::End:
			{
				currentFrame++;
				if (currentFrame >= frames.size())
				{
					currentFrame = frames.size() - 1;
					Stop();
				}
			}
			break;
			case AnimationType::Loop:
			{
				currentFrame = (currentFrame + 1) % frames.size();
			}
			break;
			case AnimationType::Reverse:
			break;
		}
	}
	frames[currentFrame].Image->Update();
}

void Clip::Render()
{
	frames[currentFrame].Image->Render();
}

void Clip::Play()
{
	bPlay = true;
	totalTime = 0.0f;
	currentFrame = 0;
}

void Clip::Stop()
{
	bPlay = false;
}

void Clip::SaveClip(wstring savePath)
{
	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLDeclaration* decl = document->NewDeclaration();

	Xml::XMLElement* root = document->NewElement(String::ToString(name).c_str());
	root->SetAttribute("xmlns:xsl", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	int i = 0;
	for (FrameData frame : frames)
	{
		string frameCount = "frame" + to_string(i++);
		Xml::XMLElement* element = document->NewElement(frameCount.c_str());
		Xml::XMLElement* element2 = document->NewElement("Image");
		frame.Image->SaveSprite(element2);
		element->SetAttribute("Time", frame.Time);
		element->LinkEndChild(element2);
		root->LinkEndChild(element);
	}
	document->SaveFile(String::ToString(savePath + name).c_str());
}

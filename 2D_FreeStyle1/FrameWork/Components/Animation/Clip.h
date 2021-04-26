#pragma once
//Frame - Clip - Animation

struct FrameData
{
	class Sprite* Image; //출력될 이미지
	float Time; //교체 시간
};
enum class AnimationType
{
	End = 0, Loop, Reverse
};
class Clip
{
public:
	Clip(wstring name, AnimationType type = AnimationType::End);
	~Clip();

	void Update();
	void Render();

	void AddFrame(FrameData data) { frames.push_back(data); }

	void Play();
	void Stop();


	bool IsPlay() { return bPlay; }

	UINT CurrentIndex() { return currentFrame; }
	Sprite* CurrentSprite() { return frames[currentFrame].Image; }

	wstring GetName() { return name; }

	void SaveClip(wstring saveFile);
private:
	wstring name;

	vector<FrameData> frames;
	AnimationType type;

	UINT currentFrame;
	bool bPlay;

	float totalTime;
};


#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	void Update() override;
	void Speed(float move, float rotation);
	void LookAt(Vector3 pos, float scaleY);
	void BattleViewStyle(bool style);

private:
	void Pos();
	void Rot();

private:
	float move = 20.0f;
	float rotation = 0.0001f;

	Vector3 at = Vector3(0.0f, 0.0f, 0.0f);

	bool changeLookAt = false;
	bool lookStyle = false;
	bool cursorShow = true;
	float distance = 70.0f;
};
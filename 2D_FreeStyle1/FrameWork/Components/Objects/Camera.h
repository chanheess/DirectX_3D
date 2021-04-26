#pragma once
#include "./Components/Component.h"
//카메라 
class Camera : public Component
{
public:
	Camera(float* width = NULL, float* height = NULL);
	~Camera();
	void SetUI(bool b = true) { bUI = b; }

	void Ready() override;
	void Update() override;
	void Render() override;

	void Insfactor() override;

	float GetWidth() { return width; }
	float GetHeight() { return height; }

	void Shake(float power, float x, float y);
private:
	struct CB_ViewOrtho
	{
		D3DXMATRIX View;
		D3DXMATRIX Ortho;
	};
	CB_ViewOrtho viewDesc;
	CBuffer* viewBuffer;

private:
	Vector3 prev;
	float width, height; //너비 높이
	Vector3 shake;
	bool bShake;
	bool bUI;
public:
	class Transform* transform;
};


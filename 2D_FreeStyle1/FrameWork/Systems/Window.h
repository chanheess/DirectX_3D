#pragma once
class Window
{
public:
	Window();
	~Window();
	virtual void Initialize() = 0;  //초기화
	virtual void Release() = 0;		//삭제
	virtual void Update() = 0;		//업데이트
	virtual void PreRender() = 0;
	virtual void Render() = 0;		//랜더

	void Create(HINSTANCE hIns); //window 창 생성
	WPARAM Run(); //동작
protected:
	class RenderTarget* rtt;
};


#pragma once
#include <bitset> //bit단위 데이터 타입 (0, 1)
//0000 0000 

class Input
{
public:
	static Input* Get();
	static void Delete();

	bool Key(int key);			//누르고 있을때(매프레임)
	bool KeyDown(int key);		//처음 눌렀을때(1번)
	bool KeyUp(int key);		//처음 땠을때(1번)
	bool Toggle(int key);		//토글(cap lock, number lock...)

	void Update();

	void Position(D3DXVECTOR3* vec) { *vec = mouse; } //Get 함수;
	void Position(D3DXVECTOR3& vec) { mouse = vec; } //Set 함수;

	D3DXVECTOR3 ScreenPosition() { return sreenMouse; }
private:
	Input();
	~Input();

	static Input* instance;
private:
	bitset<256> up;
	bitset<256> down;

	D3DXVECTOR3 mouse;//창
	D3DXVECTOR3 sreenMouse; //화면 기준

	float width, height;
};


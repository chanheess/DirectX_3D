#pragma once
//SAVE_Load
//Xml, JSon -> 세이브 이후 수정 가능
//단점 -> 보안x , 속도가 느리다.

//binary file ->4byte 32bit 저장 2byte-> 16bit
//size -> 48bit
class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(wstring filePath, UINT openOption = CREATE_ALWAYS);
	void Close();

	void Bool(bool data);
	void Word(WORD data);
	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void Double(double data);

	void Vector2(const D3DXVECTOR2& data);
	void Vector3(const D3DXVECTOR3& data);
	void Vector4(const D3DXVECTOR4& data);

	void Color3f(const D3DXCOLOR& data);
	void Color4f(const D3DXCOLOR& data);

	void Matrix(const D3DXMATRIX& data);

	void String(const string& data);
	void Byte(void* data, UINT dataSize);
protected:
	HANDLE fileHandle;
	DWORD size;
};

//주의점 저장한 순서대로 Load를 해야 된다.

class BinaryReader
{
public:
	BinaryReader();
	~BinaryReader();

	void Open(wstring filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();

	D3DXVECTOR2 Vector2();
	D3DXVECTOR3 Vector3();
	D3DXVECTOR4 Vector4();

	D3DXCOLOR Color3f();
	D3DXCOLOR Color4f();

	D3DXMATRIX Matrix();

	string String();
	void Byte(void** data, UINT dataSize);
protected:
	HANDLE fileHandle;
	DWORD size;
};

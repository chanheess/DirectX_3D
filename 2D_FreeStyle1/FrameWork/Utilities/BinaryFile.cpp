#include "stdafx.h"
#include "BinaryFile.h"

//--------------------------------------------------------------
//	BinaryWriter
//---------------------------------------------------------------
BinaryWriter::BinaryWriter()
	: fileHandle(NULL), size(0)
{
}

BinaryWriter::~BinaryWriter()
{
}

void BinaryWriter::Open(wstring filePath, UINT openOption)
{
	assert(filePath.size() > 0);

	//fOpen(c), fStream(c++), CreateFile(WinApi)
	wstring file = L"../_Resources/Saves/" + filePath;
	fileHandle = CreateFile
	(
		file.c_str(),	//파일 이름
		GENERIC_WRITE,	//Write, Read
		0,
		NULL,
		openOption,		//옵션 
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);

}

void BinaryWriter::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

void BinaryWriter::Bool(bool data)
{
	WriteFile(fileHandle, &data, sizeof(bool), &size, NULL);
}

void BinaryWriter::Word(WORD data)
{
	WriteFile(fileHandle, &data, sizeof(WORD), &size, NULL);
}

void BinaryWriter::Int(int data)
{
	WriteFile(fileHandle, &data, sizeof(int), &size, NULL);

}

void BinaryWriter::UInt(UINT data)
{
	WriteFile(fileHandle, &data, sizeof(UINT), &size, NULL);
}

void BinaryWriter::Float(float data)
{
	WriteFile(fileHandle, &data, sizeof(float), &size, NULL);
}

void BinaryWriter::Double(double data)
{
	WriteFile(fileHandle, &data, sizeof(double), &size, NULL);
}

void BinaryWriter::Vector2(const D3DXVECTOR2 & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR2), &size, NULL);
}

void BinaryWriter::Vector3(const D3DXVECTOR3 & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR3), &size, NULL);
}

void BinaryWriter::Vector4(const D3DXVECTOR4 & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR4), &size, NULL);
}

void BinaryWriter::Color3f(const D3DXCOLOR & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR) - 4, &size, NULL);
}

void BinaryWriter::Color4f(const D3DXCOLOR & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR), &size, NULL);
}

void BinaryWriter::Matrix(const D3DXMATRIX & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXMATRIX), &size, NULL);
}

void BinaryWriter::String(const string & data)
{
	UInt(data.size());
	const char* str = data.c_str();
	WriteFile(fileHandle, str, data.size(), &size, NULL);
}

void BinaryWriter::Byte(void * data, UINT dataSize)
{
	WriteFile(fileHandle, data, dataSize, &size, NULL);
}

BinaryReader::BinaryReader()
	: fileHandle(NULL), size(0)
{
}

BinaryReader::~BinaryReader()
{
}

void BinaryReader::Open(wstring filePath)
{
	assert(filePath.size() > 0);

	//fOpen(c), fStream(c++), CreateFile(WinApi)
	wstring file = L"../_Resources/Saves/" + filePath;
	fileHandle = CreateFile
	(
		file.c_str(),	//파일 이름
		GENERIC_READ,	//Write, Read
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,		//옵션 
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);

}

void BinaryReader::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

bool BinaryReader::Bool()
{
	bool temp = false;
	ReadFile(fileHandle, &temp, sizeof(bool), &size, NULL);
	return temp;
}

WORD BinaryReader::Word()
{
	WORD temp = 0;
	ReadFile(fileHandle, &temp, sizeof(WORD), &size, NULL);
	return temp;
}

int BinaryReader::Int()
{
	int temp = 0;
	ReadFile(fileHandle, &temp, sizeof(int), &size, NULL);
	return temp;
}

UINT BinaryReader::UInt()
{
	UINT temp = 0;
	ReadFile(fileHandle, &temp, sizeof(UINT), &size, NULL);
	return temp;
}

float BinaryReader::Float()
{
	float temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(float), &size, NULL);
	return temp;
}

double BinaryReader::Double()
{
	double temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(double), &size, NULL);
	return temp;
}

D3DXVECTOR2 BinaryReader::Vector2()
{
	float x = Float();
	float y = Float();

	return D3DXVECTOR2(x, y);
}

D3DXVECTOR3 BinaryReader::Vector3()
{
	float x = Float();
	float y = Float();
	float z = Float();

	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 BinaryReader::Vector4()
{
	float x = Float();
	float y = Float();
	float z = Float();
	float w = Float();

	return D3DXVECTOR4(x,y,z,w);
}

D3DXCOLOR BinaryReader::Color3f()
{
	float r = Float();
	float g = Float();
	float b = Float();

	return D3DXCOLOR(r, g, b, 1.0f);
}

D3DXCOLOR BinaryReader::Color4f()
{
	float r = Float();
	float g = Float();
	float b = Float();
	float a = Float();

	return D3DXCOLOR(r, g, b, a);
}

D3DXMATRIX BinaryReader::Matrix()
{
	D3DXMATRIX matrix;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix.m[i][j] = Float();
		}
	}

	return matrix;
}

//문자열은 문자 끝에 NULL('\0')문자가 존재 
//string size(length) -> NULL 문자가 포함x
string BinaryReader::String()
{
	UINT size = UInt();

	char* temp = new char[size + 1];
	ReadFile(fileHandle, temp, sizeof(char) * size, &this->size, NULL);
	temp[size] = '\0';
	return temp;
}

void BinaryReader::Byte(void ** data, UINT dataSize)
{
	ReadFile(fileHandle, *data, dataSize, &size, NULL);
}

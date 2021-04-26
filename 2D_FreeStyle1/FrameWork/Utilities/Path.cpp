#include "stdafx.h"
#include "Path.h"
#include "Utilities/String.h"
bool Path::ExistFile(string path)
{
	return ExistFile(String::ToWString(path));
}

bool Path::ExistFile(wstring path)
{
	//GetFileAttrubutes -> 파일이 없으면 
	// 0000 0000  ->  1111
	DWORD fileValue = GetFileAttributes(path.c_str());
	return fileValue < 0xFFFFFFFF;
}

bool Path::ExistDirectory(wstring path)
{
	return false;
}

wstring Path::GetDirectoryName(string path)
{
	return wstring();
}

wstring Path::GetExtension(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');
	return path.substr(index + 1, path.length());
}

wstring Path::GetFileName(wstring path)
{
	return wstring();
}

wstring Path::GetFileNameWithoutExtension(wstring)
{
	return wstring();
}

void Path::CreateFolder(wstring path)
{
}

void Path::CreateFolders(wstring path)
{
}
//------------------------------------------------------------------
const wstring Path::ImageFilter		= L"Image(*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0";
const wstring Path::XmlFilter		= L"Xml(*.xml)\0*.xml\0";
const wstring Path::ShaderFilter	= L"HLSL file(*.hlsl)\0*.hlsl\0";
const wstring Path::SoundFilter		= L"Sound(*.wav;*.mp3;*.mp4)\0*.wav;*.mp3;*.mp4\0";

void Path::OpenFileDialog(wstring file, const wstring filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	String::Replace(&tempFolder, L"/", L"\\");

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter.c_str();
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = (LPWSTR)L"불러오기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		if (func != NULL)
		{
			wstring loadName = name;
			String::Replace(&loadName, L"\\", L"/");

			func(loadName);
		}
	}
}

#pragma once
class Path
{
public:
	static bool ExistFile(string path); //파일 존재 여부
	static bool ExistFile(wstring path); //파일 존재 여부
	static bool ExistDirectory(wstring path); //폴더 존재 여부 

	static wstring GetDirectoryName(string path);

	static wstring GetExtension(wstring path);

	static wstring GetFileName(wstring path);

	static wstring GetFileNameWithoutExtension(wstring);

	static void CreateFolder(wstring path);
	static void CreateFolders(wstring path);

	//--------------------------------------------------------
	const static wstring ImageFilter;
	const static wstring XmlFilter;
	const static wstring ShaderFilter;
	const static wstring SoundFilter;

	static void OpenFileDialog(
		wstring file,
		const wstring filter,
		wstring folder,
		function<void(wstring)> func,
		HWND hwnd = NULL);
};



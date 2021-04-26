#include "stdafx.h"
#include "String.h"

void String::Replace(IN OUT wstring * str, wstring comp, wstring rep)
{
	wstring temp = *str;
	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}
	*str = temp;
}

string String::ToString(wstring str)
{
	string temp;
	temp.assign(str.begin(), str.end());
	return temp;
}

wstring String::ToWString(string str)
{
	wstring temp;
	temp.assign(str.begin(), str.end());
	return temp;
}

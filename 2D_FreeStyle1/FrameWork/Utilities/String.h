#pragma once
class String
{
public:
	//str 문자열에서 comp라고 되어있는 문자(열)을 rep로 변경
	//str이라는 문자열에서 comp이라는 문자(열)을 rep으로 바꿔줌
	static void Replace(IN OUT wstring* str, wstring comp, wstring rep);

	static string ToString(wstring str);
	static wstring ToWString(string str);

	//wstring은 유니코드
	//string은 멀티바이트
};


#pragma once
#define EPSILON 0.0001f

#define FLOATEQUAL(a, b) (abs(a - b) < EPSILON)
#define FLOATZERO(a) (FLOATEQUAL(a, 0))
#define VEC3EQUAL(a, b) (FLOATEQUAL(a.x, b.x) && FLOATEQUAL(a.y, b.y) && FLOATEQUAL(a.z, b.z))
class Math
{
public:
	static const float PI;

	static float ToDegree(float radian);
	static float ToRadian(float degree);

	static float FloatLerp(float from, float to, float s);
	
	static float Random(float from, float to);

	static float Clamp(float value, float from, float to);
};


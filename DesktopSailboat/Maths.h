#pragma once
#include "math.h"


struct float2
{
	float x;
	float y;
};


struct Vector2
{
	float x;
	float y;

	Vector2 operator+(const Vector2& vec) const
	{
		Vector2 res;
		res.x = vec.x + this->x;
		res.y = vec.y + this->y;
		return res;
	}

	Vector2 operator-(const Vector2& vec) const
	{
		Vector2 res;
		res.x = vec.x - this->x;
		res.y = vec.y - this->y;
		return res;
	}

	Vector2 operator* (const Vector2& vec) const
	{
		Vector2 res;
		res.x = this->x * vec.x;
		res.y = this->y * vec.y;
		return res;
	}

	
	float Magnitude()
	{
		return sqrt(x * x + y * y);
	}

	float Dot(const Vector2& other)
	{
		return x * other.x + y * other.y;
	}

	Vector2 Cross(const Vector2& other)
	{
		return { 0.0f,0.0f };
	}

};

inline float Dot(const Vector2& a, const Vector2& b)
{
	return sqrt(a.x * b.x + a.y * b.y);
}

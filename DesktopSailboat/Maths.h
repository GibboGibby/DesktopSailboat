#pragma once
#include "math.h"


struct float2
{
	float x;
	float y;
};

struct Vector2i {
	int x;
	int y;

	Vector2i operator+(const Vector2i& vec) const
	{
		Vector2i res;
		res.x = vec.x + this->x;
		res.y = vec.y + this->y;
		return res;
	}

	Vector2i operator-(const Vector2i& vec) const
	{
		Vector2i res;
		res.x = this->x - vec.x;
		res.y = this->y - vec.y;
		return res;
	}

	Vector2i operator*(const Vector2i& vec) const
	{
		Vector2i res;
		res.x = vec.x + this->x;
		res.y = vec.y + this->y;
		return res;
	}
};


struct Vector2
{
	float x;
	float y;

	Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

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
		res.x = this->x - vec.x;
		res.y = this->y - vec.y;
		return res;
	}

	Vector2 operator* (const Vector2& vec) const
	{
		Vector2 res;
		res.x = this->x * vec.x;
		res.y = this->y * vec.y;
		return res;
	}

	Vector2 operator* (const float& val) const
	{
		Vector2 res;
		res.x = this->x * val;
		res.y = this->y * val;
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

	Vector2 Normalized()
	{
		float mag = Magnitude();
		return { x / mag, y / mag };
	}

};

inline float Dot(const Vector2& a, const Vector2& b)
{
	return sqrt(a.x * b.x + a.y * b.y);
}


struct Vector2d
{
	double x;
	double y;

	Vector2d operator+(const Vector2d& vec) const
	{
		Vector2d res;
		res.x = this->x + vec.x;
		res.y = this->y + vec.y;
		return res;
	}

	Vector2d operator-(const Vector2d& vec) const
	{
		Vector2d res;
		res.x = this->x - vec.x;
		res.y = this->y - vec.y;
		return res;
	}

	Vector2d operator* (const Vector2d& vec) const
	{
		Vector2d res;
		res.x = this->x * vec.x;
		res.y = this->y * vec.y;
		return res;
	}


	float Magnitude()
	{
		return sqrt(x * x + y * y);
	}

	float Dot(const Vector2d& other)
	{
		return x * other.x + y * other.y;
	}

	Vector2d Cross(const Vector2d& other)
	{
		return { 0.0f,0.0f };
	}
};
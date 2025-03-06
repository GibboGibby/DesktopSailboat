#pragma once
#include "Maths.h"

class GameObject
{
public:
	Vector2 Position();
	void Position(const Vector2& vec);

private:
	Vector2 position;
};
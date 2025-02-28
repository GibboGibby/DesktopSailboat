#pragma once
#include "GameObject.h"
class Camera : GameObject
{
public:
	void SetBounds(const Vector2& bounds);
	Vector2 GetBounds();

	void Update();


private:
	Vector2 cameraBounds;
};
#include "Maths.h"

struct Particle
{
	Particle(float _x, float _y) : x(_x, _y), v(0.0f, 0.0f), f(0.0f, 0.0f), rho(0.0f), p(0.0f) {}

	Vector2d x;
	Vector2d v;
	Vector2d f;

	float rho, p;
};
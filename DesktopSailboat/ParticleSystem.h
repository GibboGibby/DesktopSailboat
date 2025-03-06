#pragma once

#include "GameObject.h"
#include <memory>
#include <vector>

class Renderer;
class Window;

struct Circle
{
	Vector2 pos;
	float radius;
};

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(std::shared_ptr<Renderer> _renderer, std::shared_ptr<Window> _window) : renderer(_renderer), window(_window) {}
	ParticleSystem() : renderer(nullptr), window(nullptr) {}

	void Update();
	void Render();

	void SpawnCircle(int x, int y, float rad);

private:
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<Window> window;

	std::vector<Circle> circles;
};	
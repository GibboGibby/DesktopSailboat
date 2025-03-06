#include "ParticleSystem.h"
#include "Renderer.h"
#include "Window.h"

inline float Squared(float val) {
	return val * val;
}

void ParticleSystem::Update()
{
	float mouseX, mouseY;
	SDL_GetGlobalMouseState(&mouseX, &mouseY);
	int windowX, windowY;
	SDL_GetWindowPosition(window->GetSDLWindow(), &windowX, &windowY);

	mouseX = mouseX - windowX;
	mouseY = mouseY - windowY;
	if (mouseX < 0 || mouseY < 0) return;
	for (int i = 0; i < circles.size(); i++)
	{
		Vector2 circlePos = circles[i].pos;
		Vector2 mousePos = { mouseX, mouseY };
		if ((Squared(mouseX - circlePos.x) + Squared(mouseY - circlePos.y)) < Squared(circles[i].radius))
		{
		}


		float dx = abs(mouseX - circlePos.x);
		float dy = abs(mouseY - circlePos.y);
		float r = circles[i].radius;
		if (dx > r || dy > r) continue;
		

		Vector2 minus = circlePos - mousePos;
		Vector2 normalized = minus.Normalized();
		Vector2 newPos = mousePos + (normalized * circles[i].radius);

		circles[i].pos = newPos;
	}
}

void ParticleSystem::Render()
{

	for (int i = 0; i < circles.size(); i++)
	{
		Circle circle = circles[i];
		renderer->DrawCircle(SDL_Point{ (int)circle.pos.x, (int)circle.pos.y }, circle.radius, SDL_Color{ 0, 0, 255, 255 });
	}
}

void ParticleSystem::SpawnCircle(int x, int y, float rad)
{
	circles.push_back({ Vector2((float)x,(float)y), rad });
}


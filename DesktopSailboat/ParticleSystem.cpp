#include "ParticleSystem.h"
#include "Renderer.h"
#include "Window.h"
#include "Settings.h"

inline float Squared(float val) {
	return val * val;
}

ParticleSystem::~ParticleSystem()
{
	// Clear Particles (Delete Memory)
}

void ParticleSystem::Start()
{
	ResetBox();
}

void ParticleSystem::Update()
{
	ComputeDensityPressure();
	ComputeForces();
	Integrate();

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
			Vector2 minus = circlePos - mousePos;
			Vector2 normalized = minus.Normalized();
			Vector2 newPos = mousePos + (normalized * (circles[i].radius * 1.0f));

			circles[i].pos = newPos;
		}

		/*
		float dx = abs(mouseX - circlePos.x);
		float dy = abs(mouseY - circlePos.y);
		float r = circles[i].radius;
		if (dx > r || dy > r) continue;
		v*/
		 

		Circle a = circles[i];
		for (int j = 0; j < circles.size(); j++)
		{
			if (j == i) continue;
#
			Circle b = circles[j];
			float lowerBound = Squared(a.radius - b.radius);
			float upperBound = Squared(a.radius + b.radius);
			float val = Squared(a.pos.x - b.pos.x) + Squared(a.pos.y - b.pos.y);

			if (lowerBound <= val && val <= upperBound)
			{
				//std::cout << "Two Circles Colliding" << std::endl;
				Vector2 minus = b.pos - a.pos;
				Vector2 normalized = minus.Normalized();
				Vector2 newPos = a.pos + (normalized * (a.radius + b.radius));

				circles[j].pos = newPos;
			}
		}
	}
}

void ParticleSystem::Render()
{

	

	for (int i = 0; i < circles.size(); i++)
	{
		Circle circle = circles[i];
		renderer->DrawFilledCircle(SDL_Point{ (int)circle.pos.x, (int)circle.pos.y }, circle.radius, SDL_Color{ 51, 153, 255, 255 });
	}

	renderer->DrawLine(pb.topLeft, pb.topRight, SDL_Color{ 0, 0, 255, 255 });
	renderer->DrawLine(pb.topLeft, pb.bottomLeft, SDL_Color{ 0, 0, 255, 255 });
	renderer->DrawLine(pb.bottomLeft, pb.bottomRight, SDL_Color{ 0, 0, 255, 255 });
	renderer->DrawLine(pb.topRight, pb.bottomRight, SDL_Color{ 0, 0, 255, 255 });
}

void ParticleSystem::ResetBox()
{
	pb.topLeft = Position() + Vector2{ -g_Settings.boxWidth / 2.0f, -g_Settings.boxHeight / 2.0f };
	pb.topRight = Position() + Vector2{ g_Settings.boxWidth / 2.0f, -g_Settings.boxHeight / 2.0f };

	pb.bottomLeft = Position() + Vector2{ -g_Settings.boxWidth / 2.0f, g_Settings.boxHeight / 2.0f };
	pb.bottomRight = Position() + Vector2{ g_Settings.boxWidth / 2.0f, g_Settings.boxHeight / 2.0f };
}

void ParticleSystem::SpawnCircle(int x, int y, float rad)
{
	circles.push_back({ Vector2((float)x,(float)y), rad });
}

void ParticleSystem::HandleUserInput()
{
	
}

void ParticleSystem::UpdateParticles()
{

}

void ParticleSystem::ResetParticles()
{
}

void ParticleSystem::ClearParticles()
{
}

void ParticleSystem::SpawnParticles()
{

}

void ParticleSystem::InitSPH()
{
}

void ParticleSystem::Integrate()
{
}

void ParticleSystem::ComputeDensityPressure()
{
}

void ParticleSystem::ComputeForces()
{
}

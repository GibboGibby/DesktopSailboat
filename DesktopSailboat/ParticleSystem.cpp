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
	InitSPH();
}

void ParticleSystem::Update()
{
	ComputeDensityPressure();
	ComputeForces();
	Integrate();

	/*
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

		
		float dx = abs(mouseX - circlePos.x);
		float dy = abs(mouseY - circlePos.y);
		float r = circles[i].radius;
		if (dx > r || dy > r) continue;
		
		 

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
	*/
}

void ParticleSystem::Render()
{
	for (int i = 0; i < particles.size(); i++)
	{
		Particle& particle = particles[i];
		Vector2 pos = Position();
		renderer->DrawFilledCircle(SDL_Point{ (int)(particle.x.x), (int)(particle.x.y) }, H / 2, SDL_Color{ 51, 153, 255, 255 });
	}
	
	/*
	for (int i = 0; i < circles.size(); i++)
	{
		Circle circle = circles[i];
		renderer->DrawFilledCircle(SDL_Point{ (int)circle.pos.x, (int)circle.pos.y }, circle.radius, SDL_Color{ 51, 153, 255, 255 });
	}
	*/

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
	int HEIGHT = g_Settings.boxHeight * 3;
	int WIDTH = g_Settings.boxWidth * 3;
	for (float y = EPS; y < HEIGHT - EPS * 2.0f; y += H)
	{
		for (float x = WIDTH / 4; x <= WIDTH / 2; x += H)
		{
			if (particles.size() < DAM_PARTICLES)
			{
				float jitter = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				particles.push_back(Particle(x + jitter, y));
			}
			else
			{
				return;
			}
		}
	}
}

void ParticleSystem::Integrate()
{
	int HEIGHT = g_Settings.boxHeight * 3;
	int WIDTH = g_Settings.boxWidth * 3;
	for (auto& p : particles)
	{
		// forward Euler integration
		p.v += DT * p.f / p.rho;
		p.x += DT * p.v;

		// enforce boundary conditions
		if (p.x.x - EPS < 0.f)
		{
			p.v.x *= BOUND_DAMPING;
			p.x.x = EPS;
		}
		if (p.x.x + EPS > WIDTH)
		{
			p.v.x *= BOUND_DAMPING;
			p.x.x = WIDTH - EPS;
		}
		if (p.x.y - EPS < 0.f)
		{
			p.v.y *= BOUND_DAMPING;
			p.x.y = EPS;
		}
		if (p.x.y + EPS > HEIGHT)
		{
			p.v.y *= BOUND_DAMPING;
			p.x.y = HEIGHT - EPS;
		}
	}
}

void ParticleSystem::ComputeDensityPressure()
{
	for (auto& pi : particles)
	{
		pi.rho = 0.f;
		for (auto& pj : particles)
		{
			Vector2 rij = pj.x - pi.x;
			float r2 = rij.squaredNorm();

			if (r2 < HSQ)
			{
				// this computation is symmetric
				pi.rho += MASS * POLY6 * pow(HSQ - r2, 3.f);
			}
		}
		pi.p = GAS_CONST * (pi.rho - REST_DENS);
	}
}

void ParticleSystem::ComputeForces()
{
	for (auto& pi : particles)
	{
		Vector2 fpress(0.f, 0.f);
		Vector2 fvisc(0.f, 0.f);
		for (auto& pj : particles)
		{
			if (&pi == &pj)
			{
				continue;
			}

			Vector2 rij = pj.x - pi.x;
			float r = rij.norm();

			if (r < H)
			{
				// compute pressure force contribution
				fpress += -rij.Normalized() * MASS * (pi.p + pj.p) / (2.f * pj.rho) * SPIKY_GRAD * pow(H - r, 3.f);
				// compute viscosity force contribution
				fvisc += VISC * MASS * (pj.v - pi.v) / pj.rho * VISC_LAP * (H - r);
			}
		}
		Vector2 fgrav = G * MASS / pi.rho;
		pi.f = fpress + fvisc + fgrav;
	}
}

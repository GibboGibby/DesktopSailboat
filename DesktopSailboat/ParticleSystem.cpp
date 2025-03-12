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
		renderer->DrawFilledCircle(SDL_Point{ (int)(pb.topLeft.x + particle.x.x), (int)(pb.topLeft.y + particle.x.y) }, H / 2, SDL_Color{ 51, 153, 255, 255 });
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
	particles.clear();
	InitSPH();
}

size_t ParticleSystem::GetNumberOfParticles()
{
	return particles.size();
}

void ParticleSystem::AddForceToAllParticles(Vector2 force)
{
	for (auto& particle : particles)
	{
		particle.v.x += force.x;
		particle.v.y += force.y;
	}
}

void ParticleSystem::ClearParticles()
{
	particles.clear();
}

void ParticleSystem::SpawnParticle()
{
	double HEIGHT = g_Settings.boxHeight;
	double WIDTH = g_Settings.boxWidth;
	if (particles.size() >= g_Settings.particleCount)
	{
		std::cout << "maximum number of particles reached" << std::endl;
	}
	else
	{
		unsigned int placed = 0;
		for (float y = HEIGHT / 1.5f - HEIGHT / 5.f; y < HEIGHT / 1.5f + HEIGHT / 5.f; y += H * 0.95f)
		{
			for (float x = WIDTH / 2.f - HEIGHT / 5.f; x <= WIDTH / 2.f + HEIGHT / 5.f; x += H * 0.95f)
			{
				if (placed++ < BLOCK_PARTICLES && particles.size() < g_Settings.particleCount)
				{
					particles.push_back(Particle(x, y));
				}
			}
		}
	}
}

void ParticleSystem::InitSPH()
{
	double HEIGHT = g_Settings.boxHeight;
	double WIDTH = g_Settings.boxWidth;
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
	double HEIGHT = g_Settings.boxHeight;
	double WIDTH = g_Settings.boxWidth;
	for (auto& p : particles)
	{
		// forward Euler integration
		p.v += DT * p.f / p.rho;
		p.x += DT * p.v;

		// enforce boundary conditions
		if (p.x(0) - EPS < 0.f)
		{
			p.v(0) *= BOUND_DAMPING;
			p.x(0) = EPS;
		}
		if (p.x(0) + EPS > WIDTH)
		{
			p.v(0) *= BOUND_DAMPING;
			p.x(0) = WIDTH - EPS;
		}
		if (p.x(1) - EPS < 0.f)
		{
			p.v(1) *= BOUND_DAMPING;
			p.x(1) = EPS;
		}
		if (p.x(1) + EPS > HEIGHT)
		{
			p.v(1) *= BOUND_DAMPING;
			p.x(1) = HEIGHT - EPS;
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
			Vector2d rij = pj.x - pi.x;
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
		Vector2d fpress(0.f, 0.f);
		Vector2d fvisc(0.f, 0.f);
		for (auto& pj : particles)
		{
			if (&pi == &pj)
			{
				continue;
			}

			Vector2d rij = pj.x - pi.x;
			float r = rij.norm();

			if (r < H)
			{
				// compute pressure force contribution
				fpress += -rij.Normalized() * MASS * (pi.p + pj.p) / (2.f * pj.rho) * SPIKY_GRAD * pow(H - r, 3.f);
				// compute viscosity force contribution
				fvisc += VISC * MASS * (pj.v - pi.v) / pj.rho * VISC_LAP * (H - r);
			}
		}
		Vector2d fgrav = G * MASS / pi.rho;
		pi.f = fpress + fvisc + fgrav;
	}
}

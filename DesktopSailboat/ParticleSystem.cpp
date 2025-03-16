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
		renderer->DrawFilledCircle(SDL_Point{ (int)(pb.topLeft.x + particle.x.x), (int)(pb.topLeft.y + particle.x.y) }, SIMULATION.KernelHeight / 2, SDL_Color{ 51, 153, 255, 255 });
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
	pb.topLeft = Position() + Vector2{ -g_Settings.app.boxWidth / 2.0f, -g_Settings.app.boxHeight / 2.0f };
	pb.topRight = Position() + Vector2{ g_Settings.app.boxWidth / 2.0f, -g_Settings.app.boxHeight / 2.0f };

	pb.bottomLeft = Position() + Vector2{ -g_Settings.app.boxWidth / 2.0f, g_Settings.app.boxHeight / 2.0f };
	pb.bottomRight = Position() + Vector2{ g_Settings.app.boxWidth / 2.0f, g_Settings.app.boxHeight / 2.0f };
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
	double HEIGHT = g_Settings.app.boxHeight;
	double WIDTH = g_Settings.app.boxWidth;
	if (particles.size() >= g_Settings.app.particleCount)
	{
		std::cout << "maximum number of particles reached" << std::endl;
	}
	else
	{
		unsigned int placed = 0;
		for (float y = HEIGHT / 1.5f - HEIGHT / 5.f; y < HEIGHT / 1.5f + HEIGHT / 5.f; y += SIMULATION.KernelHeight * 0.95f)
		{
			for (float x = WIDTH / 2.f - HEIGHT / 5.f; x <= WIDTH / 2.f + HEIGHT / 5.f; x += SIMULATION.KernelHeight * 0.95f)
			{
				if (placed++ < g_Settings.sim.BlockParticles && particles.size() < g_Settings.app.particleCount)
				{
					particles.push_back(Particle(x, y));
				}
			}
		}
	}
}

void ParticleSystem::InitSPH()
{
	double HEIGHT = g_Settings.app.boxHeight;
	double WIDTH = g_Settings.app.boxWidth;
	for (float y = SIMULATION.EPS; y < HEIGHT - SIMULATION.EPS * 2.0f; y += SIMULATION.KernelHeight)
	{
		for (float x = WIDTH / 4; x <= WIDTH / 2; x += SIMULATION.KernelHeight)
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
	double HEIGHT = g_Settings.app.boxHeight;
	double WIDTH = g_Settings.app.boxWidth;
	for (auto& p : particles)
	{
		// forward Euler integration
		p.v += SIMULATION.DT * p.f / p.rho;
		p.x += SIMULATION.DT * p.v;

		// enforce boundary conditions
		if (p.x(0) - SIMULATION.EPS < 0.f)
		{
			p.v(0) *= SIMULATION.BoundDamping;
			p.x(0) = SIMULATION.EPS;
		}
		if (p.x(0) + SIMULATION.EPS > WIDTH)
		{
			p.v(0) *= SIMULATION.BoundDamping;
			p.x(0) = WIDTH - SIMULATION.EPS;
		}
		if (p.x(1) - SIMULATION.EPS < 0.f)
		{
			p.v(1) *= SIMULATION.BoundDamping;
			p.x(1) = SIMULATION.EPS;
		}
		if (p.x(1) + SIMULATION.EPS > HEIGHT)
		{
			p.v(1) *= SIMULATION.BoundDamping;
			p.x(1) = HEIGHT - SIMULATION.EPS;
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

			if (r2 < SIMULATION.HSQ)
			{
				// this computation is symmetric
				pi.rho += SIMULATION.Mass * SIMULATION.POLY6 * pow(SIMULATION.HSQ - r2, 3.f);
			}
		}
		pi.p = SIMULATION.GasConst * (pi.rho - SIMULATION.RestDensity);
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

			if (r < SIMULATION.KernelHeight)
			{
				// compute pressure force contribution
				fpress += -rij.Normalized() * SIMULATION.Mass * (pi.p + pj.p) / (2.f * pj.rho) * SIMULATION.SPIKY_GRAD * pow(SIMULATION.KernelHeight - r, 3.f);
				// compute viscosity force contribution
				fvisc += SIMULATION.Viscosity * SIMULATION.Mass * (pj.v - pi.v) / pj.rho * SIMULATION.VISC_LAP * (SIMULATION.KernelHeight - r);
			}
		}
		Vector2d fgrav = Vector2d{ g_Settings.sim.Gravity.x, g_Settings.sim.Gravity.y } * SIMULATION.Mass / pi.rho;
		//Vector2d fgrav = G * MASS / pi.rho;
		pi.f = fpress + fvisc + fgrav;
	}
}

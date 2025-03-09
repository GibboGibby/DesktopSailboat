#pragma once

#include "GameObject.h"
#include <memory>
#include <vector>
#include "Particle.h"
#define _USE_MATH_DEFINES
#include "math.h"

class Renderer;
class Window;

struct Circle
{
	Vector2 pos;
	float radius;
};

struct ParticleBox
{
	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomLeft;
	Vector2 bottomRight;
};

// solver parameters
const static Vector2d G(0.f, 10.f);   // external (gravitational) forces
const static float REST_DENS = 300.f;  // rest density
const static float GAS_CONST = 2000.f; // const for equation of state
const static float H = 12.f;           // kernel radius
const static float HSQ = H * H;        // radius^2 for optimization
const static float MASS = 2.5f;        // assume all particles have the same mass
const static float VISC = 200.f;       // viscosity constant
const static float DT = 0.0007f;       // integration timestep

// smoothing kernels defined in Müller and their gradients
// adapted to 2D per "SPH Based Shallow Water Simulation" by Solenthaler et al.
const static float POLY6 = 4.f / (M_PI * pow(H, 8.f));
const static float SPIKY_GRAD = -10.f / (M_PI * pow(H, 5.f));
const static float VISC_LAP = 40.f / (M_PI * pow(H, 5.f));

// simulation parameters
const static float EPS = H; // boundary epsilon
const static float BOUND_DAMPING = -0.5f;

const int DAM_PARTICLES = 250;
const static int BLOCK_PARTICLES = 100;

class ParticleSystem : public GameObject
{
public:
	ParticleSystem(std::shared_ptr<Renderer> _renderer, std::shared_ptr<Window> _window) : renderer(_renderer), window(_window) {}
	ParticleSystem() : renderer(nullptr), window(nullptr) {}

	~ParticleSystem();

	void Start();
	void Update();
	void Render();

	void ResetBox();

	void SpawnCircle(int x, int y, float rad);

	void SpawnParticle();
	void ResetParticles();

	void AddForceToAllParticles(Vector2 force);
private:
	void HandleUserInput();
	void UpdateParticles();

	void ClearParticles();

	void InitSPH();
	void Integrate();
	void ComputeDensityPressure();
	void ComputeForces();

private:
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<Window> window;

	std::vector<Circle> circles;

	std::vector<Particle> particles;

	float boxHeight = 200;
	float boxWidth = 300;

	ParticleBox pb;
};	
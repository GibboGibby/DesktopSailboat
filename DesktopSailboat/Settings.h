#pragma once
#include "SDL3/SDL_iostream.h"
#include "Maths.h"
#define _USE_MATH_DEFINES
#include "math.h"

namespace DesktopSailboat
{

	struct ApplicationSettings
	{
		int particleCount = 20000;

		bool limitFPS = false;
		bool limitSimFPS = true;

		int fpsLimit = 60;
		int simFpsLimit = 60;

		float boxHeight = 200;
		float boxWidth = 300;
	};

	struct SimulationSettings
	{
		Vector2 Gravity = { 0.f, 10.f };
		float RestDensity = 300.f;
		float GasConst = 2000.f;
		float KernelHeight = 12.f;
		float HSQ = KernelHeight * KernelHeight;
		float Mass = 2.5f;
		float Viscosity = 200.f;
		float DT = 0.0007f;

		float EPS = KernelHeight;
		float BoundDamping = -0.5f;

		int BlockParticles = 100;


		float POLY6 = static_cast<float>(4.f / (M_PI * pow(KernelHeight, 8.f)));
		float SPIKY_GRAD = static_cast<float>(-10.f / (M_PI * pow(KernelHeight, 5.f)));
		float VISC_LAP = static_cast<float>(40.f / (M_PI * pow(KernelHeight, 5.f)));
	};

	struct Settings
	{
		ApplicationSettings app;
		SimulationSettings sim;
	};
}

extern DesktopSailboat::Settings g_Settings;

namespace DesktopSailboat
{


	inline void SaveSettings()
	{
		SDL_SaveFile("save_data.gib", &g_Settings, sizeof(Settings));
	}

	inline void LoadSettings()
	{
		size_t size = sizeof(Settings);
		void* temp = SDL_LoadFile("save_Data.gib", &size);
		if (temp != NULL)
		{
			memcpy(&g_Settings, temp, size);
		}
		SDL_free(temp);

	}

	inline void ResetSettings()
	{
		g_Settings = {};
	}

}



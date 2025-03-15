#pragma once
#include "SDL3/SDL_iostream.h"
#include "Maths.h"

namespace DesktopSailboat
{

	struct ApplicationSettings
	{
		int particleCount = 20000;
		float viscosity = 5.0f;

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

		int BlockParticles = 100;
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



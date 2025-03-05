#pragma once
#include "SDL3/SDL_iostream.h"

namespace DesktopSailboat
{

	struct Settings
	{
		int particleCount = 20000;
		float viscosity = 5.0f;

		bool limitFPS = false;
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



#pragma once
#include "SDL3/SDL.h"

class EventHandler
{
public:
	EventHandler();
	void HandleEvents(bool& isRunning);

private:
	void ProcessEvent(SDL_Event& event);
};
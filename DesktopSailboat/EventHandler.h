#pragma once
#include "SDL3/SDL.h"
#include "Window.h"

class EventHandler
{
public:
	EventHandler(std::shared_ptr<Window> window);
	void HandleEvents(bool& isRunning);

private:
	void ProcessEvent(SDL_Event& event);
	std::shared_ptr<Window> window;
};
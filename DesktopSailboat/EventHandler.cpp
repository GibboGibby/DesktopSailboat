#include "EventHandler.h"
#include "ImGUI/imgui_impl_sdl3.h"

EventHandler::EventHandler(std::shared_ptr<Window> window) : window(window)
{
}

void EventHandler::HandleEvents(bool& isRunning)
{
	SDL_Event event;
	ProcessEvent(event);
	if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window->GetSDLWindow()))
	{
		isRunning = false;
	}
	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		if (event.key.key == SDLK_ESCAPE)
		{
			isRunning = false;
		}
	}
}

void EventHandler::ProcessEvent(SDL_Event& event)
{
	SDL_PollEvent(&event);
	ImGui_ImplSDL3_ProcessEvent(&event);
}

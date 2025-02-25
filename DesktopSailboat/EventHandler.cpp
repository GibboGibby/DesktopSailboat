#include "EventHandler.h"
#include "ImGUI/imgui_impl_sdl3.h"

EventHandler::EventHandler()
{
}

void EventHandler::HandleEvents(bool& isRunning)
{

}

void EventHandler::ProcessEvent(SDL_Event& event)
{
	ImGui_ImplSDL3_ProcessEvent(&event);
}

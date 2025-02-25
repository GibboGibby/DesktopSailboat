#include "Application.h"

Application::Application() : window(nullptr), renderer(nullptr), eventHandler(nullptr), isRunning(true)
{
}

Application::~Application()
{
}

bool Application::Init()
{
	window = std::make_shared<Window>();
	renderer = std::make_shared<Renderer>(window);
	eventHandler = std::make_shared<EventHandler>();
	clickInput = std::make_shared<ClickInput>();


	if (window != nullptr && renderer != nullptr && eventHandler != nullptr)
		return true;
	else
		return false;
}

void Application::Run()
{
	// Game Loop
	while (isRunning)
	{
		eventHandler->HandleEvents(isRunning);
		float mx, my;
		SDL_MouseButtonFlags flags = SDL_GetGlobalMouseState(&mx, &my);
		clickInput->UpdateCurrentInput(flags);


		clickInput->UpdatePrevInput();
	}
}

void Application::Shutdown()
{
	// Appropriate cleanup. Should mostly be done on the side of the particle system as I am using shared pointers

}

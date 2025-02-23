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

	if (window != nullptr && renderer != nullptr && eventHandler != nullptr)
		return true;
	else
		return false;
}

void Application::Run()
{
	// Game Loop
}

void Application::Shutdown()
{
	// Appropriate cleanup. Should mostly be done on the side of the particle system as I am using shared pointers

}

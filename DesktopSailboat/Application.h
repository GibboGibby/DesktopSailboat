#pragma once
#include "Window.h"
#include "Renderer.h"
#include "EventHandler.h"
#include <memory>

class Application
{
public:
	Application();
	~Application();

	bool Init();
	void Run();
	void Shutdown();
public:


private:

private:
	bool isRunning;
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<EventHandler> eventHandler;

};
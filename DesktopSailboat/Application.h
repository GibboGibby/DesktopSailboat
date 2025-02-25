#pragma once
#include "Window.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "ClickInput.h"
#include "GuiRenderer.h"
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
	bool IsPointInsideObject(int x, int y, SDL_Rect object);
private:
	bool isRunning;
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<GuiRenderer> guiRenderer;
	std::shared_ptr<EventHandler> eventHandler;
	std::shared_ptr<ClickInput> clickInput;

};
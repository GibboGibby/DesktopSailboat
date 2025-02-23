#pragma once
#include <SDL3/SDL.h>
#include "Maths.h"
#include "Windows.h"
#include <string>
#include <iostream>

class Window
{

public:
	
	Window(int width = 1920, int height = 1080, std::string title = "Desktop Sailboat");
	~Window();

	HWND GetWindowHandle();
	void SetWindowPosition(Vector2i pos);
	Vector2i GetWindowPosition();
	SDL_Window* GetSDLWindow();
private:
	HWND windowHandle = NULL;
	Vector2i windowPos;
	std::string windowName;

	SDL_Window* window;
};
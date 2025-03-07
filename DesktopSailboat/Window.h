#pragma once
#include <SDL3/SDL.h>
#include "Maths.h"
#include "Windows.h"
#include <string>
#include <iostream>

class Window
{

public:
	
	Window(int width = 600, int height = 600, std::string title = "Desktop Sailboat");
	~Window();

	HWND GetWindowHandle();
	void SetWindowPosition(Vector2i pos);
	Vector2i GetWindowPosition();
	SDL_Window* GetSDLWindow();

	Vector2 GetWindowSize();

	void EnableClickThrough();
	void DisableClickThrough();
	void PassClickThrough(int x, int y);
private:
	float width;
	float height;

	HWND windowHandle = NULL;
	Vector2i windowPos;
	std::string windowName;

	SDL_Window* window;
};
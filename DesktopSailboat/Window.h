#pragma once
#include "Maths.h"
#include "Windows.h"

class Window
{

public:
	Window();
	~Window();

	HWND GetWindowHandle();
	void SetWindowPosition(Vector2i pos);
	Vector2i GetWindowPosition();
private:
	HWND windowHandle;
	Vector2i windowPos;
};
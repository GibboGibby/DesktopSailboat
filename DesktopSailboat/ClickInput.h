#pragma once
#include "SDL3/SDL.h"

enum GibMouseButtons
{
	LeftMouseButton = 0,
	RightMouseButton = 1,
	MiddleMouseButton = 2
};

class ClickInput
{
private:
	bool prev[3];
	bool current[3];

public:
	ClickInput();

	void UpdateCurrentInput(const SDL_MouseButtonFlags& flags);
	void UpdatePrevInput();

	bool IsMouseButtonDown(GibMouseButtons button);
	bool IsMouseButton(GibMouseButtons button);
	bool IsMouseButtonUp(GibMouseButtons button);
};
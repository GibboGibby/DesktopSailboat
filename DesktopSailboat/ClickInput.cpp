#include "ClickInput.h"
#include <cstring>

ClickInput::ClickInput()
{
	std::memset(&prev, 0, 3 * sizeof(bool));
	std::memset(&current, 0, 3 * sizeof(bool));

}

void ClickInput::UpdateCurrentInput(const SDL_MouseButtonFlags& flags)
{
	current[LeftMouseButton] = (flags & SDL_BUTTON_LMASK);
	current[RightMouseButton] = (flags & SDL_BUTTON_RMASK);
	current[MiddleMouseButton] = flags & SDL_BUTTON_MMASK;
}

void ClickInput::UpdatePrevInput()
{
	memcpy(prev, current, 3 * sizeof(bool));
}

bool ClickInput::IsMouseButtonDown(GibMouseButtons button)
{
	return (current[button] == true && prev[button] == false);
}

bool ClickInput::IsMouseButton(GibMouseButtons button)
{
	return current[button];
}

bool ClickInput::IsMouseButtonUp(GibMouseButtons button)
{
	return !current[button] && prev[button];
}
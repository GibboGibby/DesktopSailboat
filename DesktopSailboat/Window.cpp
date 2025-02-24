#include "Window.h"

Window::Window(int width, int height, std::string title)
{
    windowPos = { width, height };
    windowName = title;

    window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);

    if (window == nullptr) {
        std::cerr << "Window Could Not Be Created" << std::endl;
        exit(1);
    }
}

Window::~Window()
{

}

HWND Window::GetWindowHandle()
{
    if (windowHandle == NULL)
    {
        windowHandle = FindWindowA(NULL, windowName.c_str());
    }
    return windowHandle;
}

void Window::SetWindowPosition(Vector2i pos)
{
    windowPos = pos;
    SDL_SetWindowPosition(window, pos.x, pos.y);
}

Vector2i Window::GetWindowPosition()
{
    return windowPos;
}

SDL_Window* Window::GetSDLWindow()
{
    return window;
}

void Window::EnableClickThrough()
{
    HWND hwnd = GetWindowHandle();
    LONG_PTR exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
}

void Window::DisableClickThrough()
{
    HWND hwnd = GetWindowHandle();
    LONG_PTR exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TRANSPARENT);
}


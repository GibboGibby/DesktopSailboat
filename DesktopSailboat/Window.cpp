#include "Window.h"

Window::Window(int width, int height, std::string title)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL failed to init" << std::endl;
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, SDL_GetError());
        exit(1);
    }

    windowPos = { width, height };
    windowName = title;

    window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_ALWAYS_ON_TOP);
    //window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);

    if (window == nullptr) {
        std::cerr << "Window Could Not Be Created" << std::endl;
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
        exit(1);
    }

    SDL_ShowWindow(window);
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

void Window::PassClickThrough(int x, int y)
{
    EnableClickThrough();  // Temporarily allow clicks to pass through
    //mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
    DisableClickThrough(); // Restore normal click handling
}

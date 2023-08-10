#include <Windows.h>
#include "window.h"

void gmain();

void window(const char* appName, int width, int height)
{
    createWindow(appName, width, height);
}

int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
    gmain();

    return 0;
}
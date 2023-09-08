#include<Windows.h>
#include"window.h"
#include"graphic.h"

void gmain();

void window(const char* appName, int windowWidth, int windowHeight)
{
    createWindow(appName, windowWidth, windowHeight);
    createGraphic();
}

int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
    gmain();

    destroyGraphic();
    destroyWindow();

    return 0;
}
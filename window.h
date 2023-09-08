#pragma once

bool quit();
void closeWindow();
bool escKeyPressed();
float getWidth();
float getHeight();
int getMouseWheel();

void createWindow(const char* appName, int windowWidth, int windowHeight);
void destroyWindow();


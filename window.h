#pragma once

bool quit();
void closeWindow();
bool escKeyPressed();
void hideCursor();
void showCursor();
float getWidth();
float getHeight();

void createWindow(const char* appName, int windowWidth, int windowHeight);
void destroyWindow();


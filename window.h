#pragma once

bool quit();
void closeWindow();
bool escKeyPressed();
void hideCursor();
void showCursor();

void createWindow(const char* appName, int windowWidth, int windowHeight);
void destroyWindow();

float getWidth();
float getHeight();


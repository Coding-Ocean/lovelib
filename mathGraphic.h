#pragma once
void mathAxis(float ox, float oy, float scl, float thickness=0.02f, int order=10);
void mathCircle(float x, float y, float diameter);
void mathLine(float sx, float sy, float ex, float ey, float thickness);
void mathArrow(float sx, float sy, float ex, float ey, float thickness, float size=0.2f);
void mathGraph(float (*f)(float), float inc = 0.1f, float thickness = 0.04f, float diameter = 0.04f);
float getMathMouseX();
float getMathMouseY();

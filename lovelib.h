#pragma once
#include "window.h"
#include "graphic.h"
#include "VERTEX.h"
#include "MAT.h"
#include "VEC.h"
#include "primitive.h"
#include "input.h"
#include "mathGraphic.h"

#define width getWidth()
#define height getHeight()
#define mouseX getMouseX()
#define mouseY getMouseY()
#define mouseVx getMouseVx()
#define mouseVy getMouseVy()
#define mouseWheel getMouseWheel()
#define mathMouseX getMathMouseX()
#define mathMouseY getMathMouseY()

void window(const char* appName, int windowWidth, int windowHeight);

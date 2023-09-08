#pragma once
#include "window.h"
#include "graphic.h"
#include "VERTEX.h"
#include "MAT.h"
#include "VEC.h"
#include "primitive.h"
#include "input.h"

#define width getWidth()
#define height getHeight()

void window(const char* appName, int windowWidth, int windowHeight);

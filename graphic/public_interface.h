#pragma once

#include <SDL2/SDL.h>

static const int SIM_X_SIZE = 512;
static const int SIM_Y_SIZE = 512;

void simInit();
void simExit();
void simFlush();
void simPutPixel(int x, int y, int argb);
int simRand();
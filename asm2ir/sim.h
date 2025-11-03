#pragma once

static const int SIM_X_SIZE = 512;
static const int SIM_Y_SIZE = 256;

void simInit();
void simExit();
void simFlush();
void simPutPixel(int x, int y, int argb);
int simRand();
int simAbs(int x);
int simMax(int x, int y);
int simMin(int x, int y);
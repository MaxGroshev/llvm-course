#include "main_ability.h"

void drawLine(int x, int y, int v_x, int v_y) {
    const int  stepForLine = 10;
    int x_len = v_x;
    int y_len = v_y;
    for (int step = 0; step < stepForLine; step++) {
        simPutPixel(x + x_len * step / stepForLine, y + y_len * step / stepForLine, 0x78DBE2);
    }
}

void clearGeneratedWall(int genWallCoord) {
    for (int i = 0; i < SIM_Y_SIZE; i++) {
        simPutPixel(genWallCoord, i, 0x000001);
        simPutPixel(genWallCoord, i, 0x000001);
    }
}

int generateRandomWall() {
    int x = (simRand() % SIM_X_SIZE);
    for (int i = 0; i < SIM_Y_SIZE; i++) {
        simPutPixel(x, i, 0xFFDC33);
        simPutPixel(x+1, i, 0xFFDC33);
    }

    return x;
}

int nearTheWall(int genWallCoord, int x, int v_x) {
    if(((genWallCoord - x < 10 && v_x > 0) || (genWallCoord < 10 && v_x < 0))) {
        return 1;
    }
    return 0;
}


int app() {
    int genWallCoord = generateRandomWall();
    int x = (simRand() % SIM_X_SIZE);
    int y = (simRand() % SIM_Y_SIZE);
    int v_x = simRand() % 20;
    int v_y = simRand() % 20;
    while (1) {
        drawLine(x, y, v_x, v_y);
        x += v_x;
        y += v_y;
        if(nearTheWall(genWallCoord, x, v_x)) {
            clearGeneratedWall(genWallCoord);
            genWallCoord = generateRandomWall();
            v_x = -v_x;
        }
        else if ((SIM_X_SIZE - x < 20 && v_x > 0) || (x < 20 && v_x < 0)) {
            v_x = -v_x;
        }
        else if ((SIM_Y_SIZE - y < 20 && v_y > 0) || (y < 20 && v_y < 0)) {
            v_y = -v_y;
        }
        simFlush();
    }
    return 0;   
}
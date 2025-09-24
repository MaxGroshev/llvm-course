#include "main_ability.h"

void drawLine(int x, int y, int v_x, int v_y, int color) {
    const int  stepForLine = 100;
    int x_len = v_x;
    int y_len = v_y;
    for (int step = 0; step < stepForLine; step++) {
        simPutPixel(x + x_len * step / stepForLine, y + y_len * step / stepForLine, color);
    }
}

int app() {
    int x = (simRand() % SIM_X_SIZE);
    int y = (simRand() % SIM_Y_SIZE);
    int v_x = simRand() % 20;
    int v_y = simRand() % 20;
    int color = simRand();
    while (1) {
        drawLine(x, y, v_x, v_y, color);
        simFlush();
        x += v_x;
        y += v_y;
        if ((SIM_X_SIZE - x < 20 && v_x > 0) || (x < 20 && v_x < 0)) {
            color = simRand();
            v_x = -v_x;
        }
        if ((SIM_Y_SIZE - y < 20 && v_y > 0) || (y < 20 && v_y < 0)) {
            color = simRand();
            v_y = -v_y;
        }
    }
    return 0;   
}
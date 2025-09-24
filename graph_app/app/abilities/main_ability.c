#include "main_ability.h"

int getBarrierCenter() {
    return simRand() % (SIM_Y_SIZE - 10); // -10 to always stay inside the borders
}

int generateNewBarrier(int x_max, int y_max, int scene[][x_max]) {
    const int gateSize = SIM_Y_SIZE / 7;
    int barColor  = 0x72C908; // #72C908
    int barCenter = getBarrierCenter();

    for (int i = 0; i < y_max; i++) {
        if (simAbs(barCenter - i) >= gateSize) {
            scene[i][x_max - 2] = barColor;
        }
    }
    return barCenter;
}

int app() {
    const int gateSize = SIM_Y_SIZE / 7;
    int scene[SIM_Y_SIZE][SIM_X_SIZE] = {0};
    int barCenter = getBarrierCenter();
    int birdColor = 0xFFC908; // #FFC908
    int x_bird    = SIM_X_SIZE / 8 * 7 - 1;
    int y_bird    = barCenter;
    
    scene[y_bird][SIM_X_SIZE / 2 - 1] = birdColor; 
    for (int cnt = 0;; cnt++) {
        for(int i = 0; i < SIM_Y_SIZE; i++) {
            for(int j = 1; j < SIM_X_SIZE - 1; j++) {
                simPutPixel(j - 1, i, scene[i][j]); 
                if (x_bird != j || y_bird != i) {
                    scene[i][j] = scene[i][j + 1];
                }
            }
        }
        if (cnt % (SIM_X_SIZE / 4) == 0) {
            barCenter = generateNewBarrier(SIM_X_SIZE, SIM_Y_SIZE, scene);
            for (int i = simMin(y_bird, barCenter); i < simMax(y_bird, barCenter); i++) {
                scene[i][x_bird] = birdColor;
            }
            y_bird = barCenter;
            scene[y_bird][x_bird] = birdColor;
        }
        simFlush();
    }
    return 0;   
}
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "main.h"
#include "libs/armmath.h"
#include "libs/zint/zint.h"
#include "libs/zint/common.h"

#define BORDER 3
int qr_code(struct zint_symbol *symbol, unsigned char source[], int length);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int8_t colors[7][3] = {
    { 255, 0, 0 },
    { 0, 0, 255 },
    { 0, 255, 0 },
    { 127, 127, 0 },
    { 0, 127, 127 },
    { 127, 0, 127 },
    { 0, 0, 0 }
};
int color = 0;

static void init(void) {
    lcdFillRGB(255, 255, 255);
    color = 0;
}

static void deinit(void) {
}

static uint8_t tick(void) {
    int time = getSysTick() / 1000;
    char text[128];
    snprintf(text, 127, "%i", time);
    color++;
    if (colors[color][0] == 0 && (colors[color][1] == 0) && (colors[color][2] == 0))
        color = 0;

    struct zint_symbol zs;
    memset(&zs, 0, sizeof(zs));
    zs.input_mode = DATA_MODE;
    zs.option_1 = 3;
    /* zs.option_2 = 2; */
    int qr_res = qr_code(&zs, text, strlen(text));
    if (qr_res) {
        /* printf("Error: %i\n", qr_res); */
    }

    int zoom = MAX(1, MIN(LED_WIDTH / (zs.width + (BORDER * 2)), LED_HEIGHT / (zs.rows + (BORDER * 2))));

    int x0 = (LED_WIDTH - zoom * zs.width) / 2;
    int y0 = (LED_HEIGHT - zoom * zs.rows) / 2;
    for(int y = 0; y < zs.width; y++) {
        for(int x = 0; x < zs.rows; x++) {
            for(int y1 = y * zoom; y1 < (y + 1) * zoom; y1++) {
                for(int x1 = x * zoom; x1 < (x + 1) * zoom; x1++) {
                    if (module_is_set(&zs, x, y)) {
                        setLedXY(x0 + x1, y0 + y1, colors[color][0], colors[color][1], colors[color][2]);
                    } else {
                        setLedXY(x0 + x1, y0 + y1, 255, 255, 255);
                    }
                }
            }
        }
    }
        
    time++;
    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
    void constructor(void) {
    registerAnimation(init,tick,deinit, 1000, 2000);
}

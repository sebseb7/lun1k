/* Based on http://mbostock.github.com/protovis/ex/bzr.js */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "main.h"


#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

float *bzr_a, *bzr_b, *bzr_c;


static void init(void) {
    bzr_a = malloc(LED_WIDTH * LED_HEIGHT * sizeof(*bzr_a));
    bzr_b = malloc(LED_WIDTH * LED_HEIGHT * sizeof(*bzr_b));
    bzr_c = malloc(LED_WIDTH * LED_HEIGHT * sizeof(*bzr_c));

    for(int y = 0, p = 0; y < LED_HEIGHT; y++) {
        for (int x = 0; x < LED_WIDTH; x++, p++) {
            bzr_a[p] = (float)rand() / RAND_MAX;
            bzr_b[p] = (float)rand() / RAND_MAX;
            bzr_c[p] = (float)rand() / RAND_MAX;
        }
    }
}

static void deinit(void) {
    free(bzr_a);
    free(bzr_b);
    free(bzr_c);
}



static uint8_t tick(void) {
    for(int y = 0, p = 0; y < LED_HEIGHT; y++) {
        for (int x = 0; x < LED_WIDTH; x++, p++) {
            /* Compute neighbor averages, with wrap-around. */
            float sa = 0, sb = 0, sc = 0;
            for(int j = y; j < y + 3; j++) {
                for(int i = x - 1; i < x + 2; i++) {
                    int q =
                        (j < 0 ? j + LED_HEIGHT : j >= LED_HEIGHT ? j - LED_HEIGHT : j) * LED_WIDTH +
                        (i < 0 ? i + LED_WIDTH : i >= LED_WIDTH ? i - LED_WIDTH : i);
                    sa += bzr_a[q];
                    sb += bzr_b[q];
                    sc += bzr_c[q];
                }
            }
            sa /= 9;
            sb /= 9;
            sc /= 9;

            float ta = sa * (1 + sb - sc);
            float tb = sb * (1 + sc - sa);
            float tc = sc * (1 + sa - sb);
            bzr_a[p] = MIN(1, ta);
            bzr_b[p] = MIN(1, tb);
            bzr_c[p] = MIN(1, tc);
            setLedXY(x, y, 255 * bzr_a[p], 255 * bzr_b[p], 255 * bzr_c[p]);
        }
    }    

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
    void constructor(void) {
    registerAnimation(init,tick,deinit, 42, 1000);
}

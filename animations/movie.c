#include <stdlib.h>
#include <stdio.h>

#include "main.h"

extern uint8_t leds[LED_HEIGHT][LED_WIDTH][4];
static FILE *ffmpeg;

static void init(void)
{
    /* Target format could use files on SD card in rgb24. 
     * Use `ffmpeg -pix_fmts` for reference.
     */
    char cmd[128];
    snprintf(cmd, sizeof(cmd),
             "ffmpeg -i ./videofile -an -f rawvideo -vcodec rawvideo -r 20 -s %ix%i -pix_fmt rgba -",
             LED_WIDTH, LED_HEIGHT);
    ffmpeg = popen(cmd, "r");
}

static void deinit(void)
{
    pclose(ffmpeg);
}

static uint8_t tick(void) {
    /* Read directly into display */
    fread(leds, 1, 128 * 128 * 4, ffmpeg);
    /* Fix alpha before SDL blending */
    for(int y = 0; y < LED_HEIGHT; y++)
        for(int x = 0; x < LED_WIDTH; x++)
            leds[y][x][3] = 1;

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
        registerAnimation(init,tick,deinit, 50, 1000);
}

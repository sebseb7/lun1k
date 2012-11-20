#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "libs/text.h"
#include "libs/math.h"


/**
 * Helpers
 **/

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)


#define BLOCK_WIDTH 16
#define BLOCK_HEIGHT 16
#define PLAYER_X 32
#define LAND_Y LED_HEIGHT - BLOCK_HEIGHT

int8_t land[LED_WIDTH / BLOCK_WIDTH + 1];
uint8_t gap_len = 0;
int8_t x = 0, y = 0, y_acc = 0;

static void init(void) {
    int8_t i;
    for(i = 0; i < sizeof(land); i++)
        land[i] = 1;
}

static void deinit(void) {
}

static uint8_t *body_u =
    "   ooo  "
    "  o   o "
    " o o o o"
    " o     o"
    "  o   o "
    "o  ooo  "
    " o  o  o"
    "  oooooo";
static uint8_t *body_l1 =
    "   oo   "
    "   oo   "
    "   oo   "
    "  o  o  "
    " o    o "
    " o    o "
    " o    o "
    " o    o ";
static uint8_t *body_l2 =
    "   oo   "
    "   oo   "
    "   oo   "
    "   o o  "
    "   o  o "
    "   o  o "
    "  o   o "
    "  o    o";
static uint8_t *body_l3 =
    "   oo   "
    "   oo   "
    "   oo   "
    "   oo   "
    "   o o  "
    "  o  o  "
    " o   o  "
    "o    o  ";

static uint8_t *brick =
    "oooooooooooooooo"
    "       o        "
    "       o        "
    "       o        "
    "       o        "
    "       o        "
    "       o        "
    "       o        "
    "oooooooooooooooo"
    "o               "
    "o               "
    "o               "
    "o               "
    "o               "
    "o               "
    "o               ";

static uint8_t tick(void) {
    x++;
    while(x >= BLOCK_WIDTH) {
        x -= BLOCK_WIDTH;
        memcpy(land, land + 1, sizeof(land) * sizeof(*land));
        uint8_t new_land = gap_len > 1 || random() & 3 != 0;
        gap_len = new_land ? 0 : gap_len + 1;
        land[sizeof(land) - 1] = new_land;
    }
    
    uint8_t py, px;
    for(py = 0; py < LED_HEIGHT; py++)
        for(px = 0; px < LED_WIDTH; px++) {
            uint8_t sky = 0;
            if (py < LAND_Y || py >= LAND_Y + BLOCK_HEIGHT)
                sky = 1;
            else {
                uint8_t is_land = land[(px + x) / BLOCK_WIDTH];
                if (is_land) {
                    uint8_t brick_color = brick[(px + x) % BLOCK_WIDTH + (BLOCK_WIDTH * (py % BLOCK_HEIGHT))];
                    if (brick_color != ' ')
                        setLedXY(px, py, 0, 32, 0);
                    else
                        setLedXY(px, py, 95, 15, 15);
                } else
                    sky = 1;
            }
            if (sky)
                setLedXY(px, py, 127 - py, 127 - py, 127);
        }

    /* Handle player */
    /* Jump */
    if (get_key_press(KEY_A | KEY_B) && y_acc == 0)
        y_acc = -14;

    /* Gravity */
    y_acc++;
    if (y < 48)
        y += y_acc;
    uint8_t above_land1 = land[(PLAYER_X + x) / BLOCK_WIDTH];
    uint8_t above_land2 = land[(PLAYER_X + x + 7) / BLOCK_WIDTH];
    if (y > 0 && y <= 127 && (above_land1 || above_land2)) {
        y_acc = 0;
        y = 0;
    }

    /* Player, upper body */
    for(py = 0; py < 8; py++)
        for(px = 0; px < 8; px++) {
            if (body_u[px + 8 * py] != ' ')
                setLedXY(PLAYER_X + px, LAND_Y - 16 + py + y, 0, 0, 0);
        }
    uint8_t *body_l;
    switch((x >> 1) & 0x3) {
    case 0:
        body_l = body_l1;
        break;
    case 1:
        body_l = body_l2;
        break;
    case 2:
        body_l = body_l1;
        break;
    case 3:
        body_l = body_l3;
        break;
    }
    for(py = 0; py < 8; py++)
        for(px = 0; px < 8; px++) {
            if (body_l[px + 8 * py] != ' ')
                setLedXY(PLAYER_X + px, LAND_Y - 8 + py + y, 0, 0, 0);
        }

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
    void constructor(void) {
    registerAnimation(init,tick,deinit, 4, 1000);
}

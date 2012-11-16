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


static void init(void) {
}

static void deinit(void) {
}

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

static uint8_t map[MAP_WIDTH][MAP_HEIGHT] = {
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
    { 0xff,    0,    0,    0, 0xff, 0xff,    0, 0xff, 0xff, 0xff, 0xff,    0,    0,    0,    0, 0xff },
    { 0xff,    0,    0,    0,    0,    0,    0, 0xff, 0xff, 0xff, 0xff,    0,    0,    0,    0, 0xff },
    { 0xff, 0xfe, 0xfe, 0xfe, 0xfe,    0,    0,    0,    0,    0, 0xff,    0,    0,    0,    0, 0xff },
    { 0xff, 0xff, 0xff, 0xff, 0xff,    0, 0xfe, 0xfe, 0xfe,    0, 0xff,    0,    0,    0,    0, 0xff },
    { 0xff, 0xff,    0,    0,    0,    0,    0,    0,    0,    0, 0xff,    1,    0,    0,    0, 0xff },
    { 0xff, 0xff,    0,    0,    0,    0,    0,    0,    0, 0xfe, 0xff, 0xfe, 0xfe, 0xfe,    2, 0xff },
    { 0xff,    0,    0,    0, 0xfe,    0, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,    2, 0xff },
    { 0xff,    0,    0, 0xfe, 0xff,    0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,    2, 0xff },
    { 0xff,    0, 0xfe, 0xff, 0xff,    0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,    2, 0xff },
    { 0xff,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    2, 0xff },
    { 0xff, 0xfe,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    2, 0xff },
    { 0xff, 0xff, 0xfe, 0xfe, 0xfe,    0, 0xfe,    0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
    { 0xff, 0xff, 0xff, 0xff, 0xff,    0, 0xff,    0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
    { 0xff, 0xff, 0xff, 0xff, 0xff,    0, 0xff,    0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
};

#define BLOCK_W 16
#define BLOCK_H 16

uint8_t wall_tex[] = {
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0"
    "\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200"
    "\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200"
    "\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\274\0\0\274\0\0\274\0\0"
    "\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200"
    "\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200"
    "\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200"
    "\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274"
    "\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0"
    "\274\0\0\274\0\0\274\0\0"
};

uint8_t grass_wall_tex[] = {
    "\0\206\0\0\247\0\6\241\0\6\253\0\6\253\0\6\241\0\0\247\0\6\253\0\6\253\0"
    "\6\253\0\6\253\0\0k\0\6\241\0\6\253\0\0k\0\0\206\0\6\241\0""0^\0\6\241\0"
    ">y\0>y\0\6\231\0\6\241\0\6\253\0>y\0\0\206\0\0\247\0\6\231\0\6\241\0>y\0"
    "\6\231\0\0\206\0>y\0""0^\0>y\0>y\0>y\0""0^\0>y\0\25\260\17h\2268>y\0\6\253"
    "\0\6\241\0>y\0>y\0>y\0\25\260\17>y\0>y\0>y\0""0^\0""0^\0\274\0\0""0^\0,\237"
    "&h\22680^\0\6\253\0\6\241\0>y\0""0^\0""0^\0!\264\33\200\200\200,\237&,\237"
    "&,\237&,\237&\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "3\2713\6\253\0\200\200\200,\237&,\237&,\237&\274\0\0\274\0\0>y\0\200\200"
    "\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0""3\2713\200\200\200"
    ">y\0h\2268\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274"
    "\0\0\274\0\0\274\0\0\274\0\0\274\0\0\\\307\\\200\200\200\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0"
    "\274\0\0\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200\200"
    "\200\200\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0"
    "\274\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0\274\0\0\274"
    "\0\0\274\0\0\200\200\200\274\0\0\274\0\0\274\0\0\274\0\0",
};

void draw_tex_rgb(uint8_t *tex, uint8_t x1, uint8_t y1, uint8_t dx, uint8_t dy) {
    uint8_t r = tex[3 * (y1 * BLOCK_W + x1)];
    uint8_t g = tex[3 * (y1 * BLOCK_W + x1) + 1];
    uint8_t b = tex[3 * (y1 * BLOCK_W + x1) + 2];
    setLedXY(dx, dy, r, g, b);
}

uint8_t door_tex[] = {
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\360\327\177\377\360\327"
    "\177\377\360\327\177\377\360\327\177\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\360\327\177\377\360\327"
    "\177\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\360\327\177\377\360\327"
    "\177\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\360\327"
    "\177\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377P?\230"
    "\377P?\230\377\360\327\177\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\360"
    "\327\177\377\360\327\177\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0"
    "\0\377P?\230\377P?\230\377P?\230\377\360\327\177\377\360\327\177\377\0\0"
    "\0\0\0\0\0\0\0\0\0\0\343\260\0\377\360\327\177\377\0\0\0\377\0\0\0\377\0"
    "\0\0\377\0\0\0\377\0\0\0\377\200c\377\377\200c\377\377\200c\377\377\200c"
    "\377\377\200c\377\377\360\327\177\377\343\260\0\377\0\0\0\0\0\0\0\0\343\260"
    "\0\377qX\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\200c\377"
    "\377\200c\377\377\200c\377\377\200c\377\377\200c\377\377\360\327\177\377"
    "\343\260\0\377\0\0\0\0\0\0\0\0\343\260\0\377qX\0\377\0\0\0\377\0\0\0\377"
    "\0\0\0\377\0\0\0\377\0\0\0\377\200c\377\377\200c\377\377\200c\377\377\200"
    "c\377\377\200c\377\377\360\327\177\377\343\260\0\377\0\0\0\0\0\0\0\0\343"
    "\260\0\377qX\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377P?\230\377P?\230"
    "\377P?\230\377P?\230\377P?\230\377P?\230\377\360\327\177\377\343\260\0\377"
    "\0\0\0\0\0\0\0\0\343\260\0\377qX\0\377\0\0\0\377\0\0\0\377\0\0\0\377P?\230"
    "\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377\360\327"
    "\177\377\343\260\0\377\0\0\0\0\0\0\0\0\343\260\0\377qX\0\377\0\0\0\377\0"
    "\0\0\377\200c\377\377\200c\377\377\200c\377\377\200c\377\377\200c\377\377"
    "\200c\377\377\200c\377\377\200c\377\377\360\327\177\377\343\260\0\377\0\0"
    "\0\0\0\0\0\0\343\260\0\377qX\0\377\0\0\0\377\0\0\0\377\200c\377\377\200c"
    "\377\377\200c\377\377\200c\377\377\200c\377\377\200c\377\377\200c\377\377"
    "\200c\377\377\360\327\177\377\343\260\0\377\0\0\0\0\0\0\0\0\343\260\0\377"
    "qX\0\377\0\0\0\377\0\0\0\377\200c\377\377\200c\377\377\200c\377\377\200c"
    "\377\377\200c\377\377\200c\377\377\200c\377\377\200c\377\377\360\327\177"
    "\377\343\260\0\377\0\0\0\0\0\0\0\0\343\260\0\377qX\0\377\0\0\0\377P?\230"
    "\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377"
    "P?\230\377\360\327\177\377\343\260\0\377\0\0\0\0\0\0\0\0\343\260\0\377qX"
    "\0\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230\377P?\230"
    "\377P?\230\377P?\230\377P?\230\377\360\327\177\377\343\260\0\377\0\0\0\0"
    "\0\0\0\0\343\260\0\377qX\0\377qX\0\377qX\0\377qX\0\377qX\0\377qX\0\377qX"
    "\0\377qX\0\377qX\0\377qX\0\377qX\0\377\343\260\0\377\343\260\0\377\0\0\0"
    "\0\0\0\0\0\343\260\0\377\343\260\0\377\343\260\0\377\343\260\0\377\343\260"
    "\0\377\343\260\0\377\343\260\0\377\343\260\0\377\343\260\0\377\343\260\0"
    "\377\343\260\0\377\343\260\0\377\343\260\0\377\343\260\0\377\0\0\0\0",
};

uint8_t baddie_tex[] = {
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377"
    "\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\377\377"
    "\377\377\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0"
    "\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377\377\0\0"
    "\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377"
    "\377\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377"
    "\377\377\377\377\377\0\0\0\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0"
    "\0\0\0\377\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\0\0"
    "\0\377\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377"
    "\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\0\0"
    "\0\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\377"
    "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0"
    "\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0"
    "\377\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377"
    "\377\377\377\377\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377"
    "\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\377\0\0\0\377"
    "\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\377"
    "\377\377\377\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\0"
    "\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\377\0\0\0\377\377"
    "\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\377\377"
    "\377\377\377\377\377\377\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0"
    "\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\377\0\0\0\0\0\0\0\377"
    "\0\0\0\377\0\0\0\377\0\0\0\377\0\0\0\377\377\377\377\377\377\377\377\377"
    "\0\0\0\377\377\377\377\377\377\377\377\377\0\0\0\377\0\0\0\377\0\0\0\377"
    "\0\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\377\377\0\0\377\0\0\0\0\0\0\0"
    "\0\0\0\0\377\377\377\377\377\0\0\0\377\377\377\377\377\0\0\0\377\0\0\0\0"
    "\377\0\0\377\377\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\377\200"
    "\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\0\0\0\0\0\0\0\377\0\0\0\0\0\0"
    "\0\0\200\0\0\377\377\0\0\377\0\0\0\0\0\0\0\0\377\0\0\377\377\0\0\377\377"
    "\0\0\377\200\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\200\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377"
    "\200\0\0\377\200\0\0\377\200\0\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\200\0\0\377\200\0\0\377\200\0\0\377\377\0\0"
    "\377",
};

uint8_t robo_tex[] = {
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377"
    "\377\377\0\0\0\0\0\0\0\0\377\377\377\377\377\377\377\377\377\377\377\377"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\377"
    "\377\377\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\377\377\377\377"
    "\377\377\377\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\377\377\377\377\0\0\0\377\0\0\0\377\377\374\0\377"
    "\377\374\0\377\0\0\0\377\0\0\0\377\377\377\377\377\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\374\0\377\377\377\377\377\0\0\0\377"
    "\0\0\0\377\377\374\0\377\377\374\0\377\0\0\0\377\0\0\0\377\377\377\377\377"
    "\377\374\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\374\0\377\377"
    "\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374"
    "\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0"
    "\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\374\0\377\377\374\0\377\274\271"
    "\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0"
    "\377\377\374\0\377\274\271\0\377\377\374\0\377\377\374\0\377\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\377\374\0\377\274\271\0\377\377\374\0\377\377\374\0"
    "\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377"
    "\377\374\0\377\274\271\0\377\377\374\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\377\374\0\377\274\271\0\377\274\271\0\377\377\374\0\377\377\374\0\377"
    "\377\374\0\377\274\271\0\377\377\374\0\377\377\374\0\377\274\271\0\377\377"
    "\374\0\377\377\374\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\374\0\377\274"
    "\271\0\377\377\374\0\377\377\374\0\377\377\374\0\377\274\271\0\377\377\374"
    "\0\377\377\374\0\377\377\374\0\377\377\374\0\377\274\271\0\377\377\374\0"
    "\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\377\374\0\377\274\271\0\377\274\271"
    "\0\377\377\374\0\377\377\374\0\377\274\271\0\377\377\374\0\377\377\374\0"
    "\377\377\374\0\377\274\271\0\377\377\374\0\377\377\374\0\377\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\377\374\0\377\274\271\0\377\377\374\0\377\377\374\0"
    "\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377\377\374\0\377"
    "\377\374\0\377\274\271\0\377\377\374\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\377\374\0\377\301\301\301\377\301\301\301\377\301\301\301\377\377\374"
    "\0\377\377\374\0\377\377\374\0\377\377\374\0\377\301\301\301\377\301\301"
    "\301\377\301\301\301\377\377\374\0\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\301"
    "\301\301\377\207\207\207\377\207\207\207\377\207\207\207\377\301\301\301"
    "\377\377\374\0\377\377\374\0\377\301\301\301\377\207\207\207\377\207\207"
    "\207\377\207\207\207\377\301\301\301\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\301\301\301\377\207\207\207\377\207\207\207\377\207\207\207\377\301\301"
    "\301\377\0\0\0\0\0\0\0\0\301\301\301\377\207\207\207\377\207\207\207\377"
    "\207\207\207\377\301\301\301\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\301\301"
    "\301\377\207\207\207\377\207\207\207\377\207\207\207\377\301\301\301\377"
    "\0\0\0\0\0\0\0\0\301\301\301\377\207\207\207\377\207\207\207\377\207\207"
    "\207\377\301\301\301\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\301\301"
    "\301\377\301\301\301\377\301\301\301\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\301\301\301\377\301\301\301\377\301\301\301\377\0\0\0\0\0\0\0\0\0\0\0\0",
};

uint8_t ladder_tex[] = {
    "\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377f\331\377\377f\331"
    "\377\377f\331\377\377f\331\377\377f\331\377\377f\331\377\377\0\300\377\377"
    "\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331"
    "\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377"
    "\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\177\337\377\377\0\300\377"
    "\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377"
    "\0\300\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\377"
    "\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331"
    "\377\377\0\300\377\377f\331\377\377f\331\377\377f\331\377\377f\331\377\377"
    "f\331\377\377f\331\377\377\0\300\377\377\0\300\377\377\0\300\377\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377\0\300\377"
    "\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377"
    "\0\300\377\377\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "f\331\377\0f\331\377\377\0\300\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0f\331\377\377\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\377\0\300\377\377\0\300\377\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377f\331\377\377"
    "f\331\377\377f\331\377\377f\331\377\377f\331\377\377f\331\377\377\0\300\377"
    "\377\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0"
    "f\331\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300"
    "\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\177\337\377\377\0\300\377"
    "\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377"
    "\0\300\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\377"
    "\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331"
    "\377\377\0\300\377\377f\331\377\377f\331\377\377f\331\377\377f\331\377\377"
    "f\331\377\377f\331\377\377\0\300\377\377\0\300\377\377\0\300\377\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377\0\300\377"
    "\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377\0\300\377\377"
    "\0\300\377\377\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "f\331\377\0f\331\377\377\0\300\377\377\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0f\331\377\377\0\300\377\377\0\300\377\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0f\331\377\0f\331\377\377\0\300\377\377\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0f\331\377\377\0\300\377\377\0\300\377\0\0\0\0\0"
    "\0\0\0\0",
};

void draw_tex_rgba(uint8_t *tex, uint8_t x1, uint8_t y1, uint8_t dx, uint8_t dy) {
    uint8_t r = tex[4 * (y1 * BLOCK_W + x1)];
    uint8_t g = tex[4 * (y1 * BLOCK_W + x1) + 1];
    uint8_t b = tex[4 * (y1 * BLOCK_W + x1) + 2];
    uint8_t a = tex[4 * (y1 * BLOCK_W + x1) + 3];
    if (a > 0)
        setLedXY(dx, dy, r, g, b);
}

struct baddie {
    uint8_t x, y;
    uint8_t dir;
};

struct baddie baddies[] = {
    { .x = 2 * BLOCK_W, .y = 11 * BLOCK_H, .dir = 0 },
    { .x = 8 * BLOCK_W, .y = 11 * BLOCK_H, .dir = 0 },
    { .x = 13 * BLOCK_W, .y = 5 * BLOCK_H, .dir = 0 },
};

void baddie_think(struct baddie *b) {
}

int16_t playerX = 3 * BLOCK_W / 2, playerY = 2 * BLOCK_H;

static uint8_t tick(void) {
    playerX++;

    int16_t camX = playerX - (LED_WIDTH / 2);
    int16_t camY = playerY - (LED_HEIGHT / 2) + BLOCK_H;

    /* Draw map */
    for(int16_t y = camY - (camY % BLOCK_H); y < camY + LED_HEIGHT; y += BLOCK_H) {
        for(int16_t x = camX - (camX % BLOCK_W); x < camX + LED_WIDTH; x += BLOCK_W) {
            uint8_t tex = (x >= 0 && x < MAP_WIDTH * BLOCK_W &&
                           y >= 0 && y < MAP_HEIGHT * BLOCK_H) ?
                map[y / BLOCK_H][x / BLOCK_W] :
                0;
            for(uint8_t y1 = 0; y1 < BLOCK_H; y1++) {
                for(uint8_t x1 = 0; x1 < BLOCK_W; x1++) {
                    int16_t dx = x + x1 - camX;
                    int16_t dy = y + y1 - camY;
                    if (dx >= 0 && dx < LED_WIDTH &&
                        dy >= 0 && dy < LED_HEIGHT)
                        switch(tex) {
                        case 0:
                            /* Background */
                            setLedXY(dx, dy, 0, 0, 0x3f);
                            break;
                        case 1:
                            draw_tex_rgba(door_tex, x1, y1, dx, dy);
                            break;
                        case 2:
                            draw_tex_rgba(ladder_tex, x1, y1, dx, dy);
                            break;
                        case 0xfe:
                            draw_tex_rgb(grass_wall_tex, x1, y1, dx, dy);
                            break;
                        case 0xff:
                            draw_tex_rgb(wall_tex, x1, y1, dx, dy);
                            break;
                        }
                }
            }
        }
    }
    /* Baddies */
    for(uint8_t i = 0; i < sizeof(baddies); i++) {
        struct baddie *b = baddies + i;
        baddie_think(b);
        for(uint8_t y1 = 0; y1 < BLOCK_H; y1++) {
            for(uint8_t x1 = 0; x1 < BLOCK_W; x1++) {
                int16_t dx = b->x + x1 - camX;
                int16_t dy = b->y + y1 - camY;
                if (dx >= 0 && dx < LED_WIDTH &&
                    dy >= 0 && dy < LED_HEIGHT)
                    draw_tex_rgba(baddie_tex, x1, y1, dx, dy);
            }
        }
    }
    /* Draw player */
    for(uint8_t y1 = 0; y1 < BLOCK_H; y1++) {
        for(uint8_t x1 = 0; x1 < BLOCK_W; x1++) {
            int16_t dx = playerX + x1 - camX;
            int16_t dy = playerY + y1 - camY;
            draw_tex_rgba(robo_tex, x1, y1, dx, dy);
        }
    }

    return 0;
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
    void constructor(void) {
    registerAnimation(init,tick,deinit, 4, 1000);
}

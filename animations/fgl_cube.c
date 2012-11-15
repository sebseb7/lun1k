#include <stdlib.h>
#include <fgl/fgl.h>
#include "main.h"

uint8_t tex[] = {
    "\40\4\40\5\0\5@\5@\5\0\5\40\5@\5@\5@\5@\5@\3\0\5@\5@\3\40\4\0\5\3402\0\5"
    "\300;\300;\300\4\0\5@\5\300;\40\4\40\5\300\4\0\5\300;\300\4\40\4\300;\340"
    "2\300;\300;\300;\3402\300;\201\25\247l\300;@\5\0\5\300;\300;\300;\201\25"
    "\300;\300;\300;\3402\3402\0\270\3402\344,\247l\3402@\5\0\5\300;\3402\340"
    "2\243%\20\204\344,\344,\344,\344,\20\204\20\204\20\204\20\204\20\204\306"
    "5@\5\20\204\344,\344,\344,\0\270\0\270\300;\20\204\0\270\0\270\0\270\0\270"
    "\0\270\0\270\3065\20\204\300;\247l\0\270\0\270\0\270\0\270\0\270\20\204\0"
    "\270\0\270\0\270\0\270\0\270\0\270+^\20\204\0\270\0\270\0\270\0\270\0\270"
    "\0\270\0\270\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270\20\204\0\270"
    "\0\270\0\270\0\270\20\204\20\204\20\204\20\204\20\204\20\204\20\204\20\204"
    "\20\204\20\204\20\204\20\204\20\204\20\204\20\204\20\204\0\270\0\270\0\270"
    "\0\270\0\270\0\270\0\270\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270"
    "\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270\20\204\0\270\0\270\0\270"
    "\0\270\0\270\0\270\0\270\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270"
    "\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270\20\204\20\204\20\204\20"
    "\204\20\204\20\204\20\204\20\204\20\204\20\204\20\204\20\204\20\204\20\204"
    "\20\204\20\204\20\204\0\270\0\270\0\270\20\204\0\270\0\270\0\270\0\270\0"
    "\270\0\270\0\270\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270\20\204"
    "\0\270\0\270\0\270\0\270\0\270\0\270\0\270\20\204\0\270\0\270\0\270\0\270"
    "\0\270\0\270\0\270\20\204\0\270\0\270\0\270\0\270\0\270\0\270\0\270\20\204"
    "\0\270\0\270\0\270\0\270",
};

extern fgl_texture* _fgl_texture;

void draw_cube() {
    /* fgl_matrix_mode_set(FGL_MATRIX_MODEL); */
    /* fgl_matrix_identity(); */
    /*   fgl_translate(fix16_from_int(x), fix16_from_int(y), fix16_from_int(z)); */

    fgl_vertex_t tempVerts[8];

    uintptr_t i;
    for(i = 0; i < 8; i++) {
        tempVerts[i].x = ((i & 1) ? 1 : -1) << 16;
        tempVerts[i].y = ((i & 2) ? 1 : -1) << 16;
        tempVerts[i].z = ((i & 4) ? 1 : -1) << 16;
        tempVerts[i].u = ((i & 1) ? 0 : (_fgl_texture->width  - 1));
        tempVerts[i].v = ((i & 2) ? 0 : (_fgl_texture->height - 1));
        tempVerts[i].c = 0xFFFF;
    }

    fgl_vertex_t tempFace[4];

    // Front face.
    tempFace[0] = tempVerts[1];
    tempFace[1] = tempVerts[0];
    tempFace[2] = tempVerts[3];
    tempFace[3] = tempVerts[2];
    fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

    // Back face.
    tempFace[0] = tempVerts[4];
    tempFace[1] = tempVerts[5];
    tempFace[2] = tempVerts[6];
    tempFace[3] = tempVerts[7];
    fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

    // Left face.
    tempFace[0] = tempVerts[0];
    tempFace[1] = tempVerts[4];
    tempFace[2] = tempVerts[2];
    tempFace[3] = tempVerts[6];
    fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

    // Right face.
    tempFace[0] = tempVerts[5];
    tempFace[1] = tempVerts[1];
    tempFace[2] = tempVerts[7];
    tempFace[3] = tempVerts[3];
    fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

    // Top face.
    tempFace[0] = tempVerts[0];
    tempFace[1] = tempVerts[1];
    tempFace[2] = tempVerts[4];
    tempFace[3] = tempVerts[5];
    fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);

    // Bottom face.
    tempFace[0] = tempVerts[3];
    tempFace[1] = tempVerts[2];
    tempFace[2] = tempVerts[7];
    tempFace[3] = tempVerts[6];
    fgl_draw_array(FGL_TRIANGLE_STRIP, tempFace, 4);
}

void draw_wire_cube() {
    fgl_vertex_t tempVerts[8];

    uintptr_t i;
    for(i = 0; i < 8; i++) {
        tempVerts[i].x = ((i & 1) ? 1 : -1) << 16;
        tempVerts[i].y = ((i & 2) ? 1 : -1) << 16;
        tempVerts[i].z = ((i & 4) ? 1 : -1) << 16;
        tempVerts[i].u = ((i & 1) ? 0 : (_fgl_texture->width  - 1));
        tempVerts[i].v = ((i & 2) ? 0 : (_fgl_texture->height - 1));
        tempVerts[i].c = 0xFFFF;
    }

    fgl_vertex_t tempFace[4];

    // Front face.
    tempFace[0] = tempVerts[1];
    tempFace[1] = tempVerts[0];
    tempFace[2] = tempVerts[3];
    tempFace[3] = tempVerts[2];
    fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

    // Back face.
    tempFace[0] = tempVerts[4];
    tempFace[1] = tempVerts[5];
    tempFace[2] = tempVerts[6];
    tempFace[3] = tempVerts[7];
    fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

    // Left face.
    tempFace[0] = tempVerts[0];
    tempFace[1] = tempVerts[4];
    tempFace[2] = tempVerts[2];
    tempFace[3] = tempVerts[6];
    fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

    // Right face.
    tempFace[0] = tempVerts[5];
    tempFace[1] = tempVerts[1];
    tempFace[2] = tempVerts[7];
    tempFace[3] = tempVerts[3];
    fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

    // Top face.
    tempFace[0] = tempVerts[0];
    tempFace[1] = tempVerts[1];
    tempFace[2] = tempVerts[4];
    tempFace[3] = tempVerts[5];
    fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);

    // Bottom face.
    tempFace[0] = tempVerts[3];
    tempFace[1] = tempVerts[2];
    tempFace[2] = tempVerts[7];
    tempFace[3] = tempVerts[6];
    fgl_draw_array(FGL_LINE_LOOP, tempFace, 4);
}


fgl_texture* _display_buffer = NULL;


static void init(void) {
    _display_buffer = fgl_texture_create(LED_WIDTH, LED_HEIGHT);
    if (!_display_buffer) {
        printf("fgl_texture_create :-(\n");
        exit(1);
    }
    fgl_texture_clear(_display_buffer);
    fgl_draw_buffer_set(_display_buffer);

    uint16_t* tempDepth = (uint16_t*)malloc((_display_buffer->width * _display_buffer->height) << 1);
    if(tempDepth != NULL)
        fgl_depth_buffer_set(tempDepth);
    fgl_enable(FGL_DEPTH_TEST);

    fgl_matrix_mode_set(FGL_MATRIX_VIEWPORT);
    fgl_matrix_identity();
    fgl_viewport(0, 0, fix16_from_int(LED_WIDTH), fix16_from_int(LED_HEIGHT));
    fgl_matrix_mode_set(FGL_MATRIX_PROJECTION);
    fgl_matrix_identity();
    fgl_perspective((fix16_pi >> 3), fix16_div(fix16_from_int(LED_WIDTH), fix16_from_int(LED_HEIGHT)), fix16_one, fix16_from_int(1024));
    fgl_ortho_2d(-fix16_one, fix16_one, -fix16_one, fix16_one);
    fgl_matrix_mode_set(FGL_MATRIX_VIEW);
    fgl_matrix_identity();
    fgl_translate(fix16_from_int(0), fix16_from_int(0), fix16_from_int(10));
    fgl_matrix_mode_set(FGL_MATRIX_MODEL);
    fgl_matrix_identity();

    fgl_scale(fix16_from_int(1), fix16_from_int(1), fix16_from_int(1));


    fgl_enable(FGL_CULL_FACE);

    fgl_texture* tempTexture = fgl_texture_create(16, 16);
    memcpy(tempTexture->data, tex, 2 * 16 * 16 );
    fgl_texture_bind(tempTexture);
    fgl_enable(FGL_TEXTURE_2D);
}

static void deinit(void) {
    fgl_texture_delete(_display_buffer);
    _display_buffer = NULL;
}


static uint8_t tick(void) {
    fgl_clear(FGL_COLOR_BUFFER_BIT | FGL_DEPTH_BUFFER_BIT);
    draw_cube();
    fgl_rotate_y(500);
    fgl_rotate_x(500);
    fgl_rotate_z(500);
    for(uint8_t y = 0; y < LED_HEIGHT; y++)
        for(uint8_t x = 0; x < LED_WIDTH; x++) {
            uint16_t rgb = _display_buffer->data[x + y * _display_buffer->width];
            setLedXY(x, y, (rgb >> 8) & 0xfc, (rgb >> 2) & 0xf8, (rgb << 2) & 0xfc);
        }
}

static void constructor(void) CONSTRUCTOR_ATTRIBUTES
    void constructor(void) {
    registerAnimation(init,tick,deinit, 4, 1000);
}

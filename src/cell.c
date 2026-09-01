#ifndef CELL
#define CELL

#include <stdint.h>
#include <cp_base.c>

// ------------ INTERFACE -------------

#define CELL_COLOR(r, g, b) ( \
    (r & 0b11111000) << 8 | \
    (g & 0b11111100) << 3 | \
    (b & 0b11111000) >> 3 \
)

// does not include spacing between cells
#define CELL_SIZE 7 
#define CELL_SCALE 3
#define CELL_RENDER_SIZE (CELL_SIZE * CELL_SCALE)
#define CELL_PIXELS (CELL_RENDER_SIZE * CELL_RENDER_SIZE)

uint16_t *cell_atlas;
uint8_t cell_templates[] = {
    0, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,

    0, 2, 2, 2, 2, 2, 2,
    2, 0, 0, 2, 2, 2, 2,
    2, 0, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,

    0, 2, 2, 2, 2, 2, 2,
    2, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 2,
    2, 2, 2, 2, 2, 2, 2,
};

void cell_init();
void cell_generate(uint16_t lightColor, uint16_t darkColor);
void cell_render(unsigned type, unsigned x, unsigned y);

// ---------- IMPLEMENTATION ----------

void cell_init() {
    cell_atlas = cp_allocate(CELL_PIXELS * 4 * sizeof(uint16_t));
    cell_generate(CELL_COLOR(100, 176, 255), CELL_COLOR(66, 64, 255));
}

void cell_generate(const uint16_t lightColor, const uint16_t darkColor) {

}

void cell_render(const unsigned type, const unsigned x, const unsigned y) {

}

#endif
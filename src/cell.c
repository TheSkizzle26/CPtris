#ifndef CELL
#define CELL

#include <stdint.h>

// ------------ INTERFACE -------------

#define CELL_COLOR(r, g, b) ( \
    ((r) & 0b11111000) << 8 | \
    ((g) & 0b11111100) << 3 | \
    ((b) & 0b11111000) >> 3 \
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
void cell_generateAtlas(uint16_t lightColor, uint16_t darkColor);
void cell_render(unsigned type, unsigned x, unsigned y);

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

void cell_init() {
    cell_atlas = cp_allocate(CELL_PIXELS * 4 * sizeof(uint16_t));

    // empty first type
    for (unsigned i = 0; i < CELL_PIXELS; i++)
        cell_atlas[i] = 0;
}

void cell_generateAtlas(const uint16_t lightColor, const uint16_t darkColor) {
    const uint16_t palette[3] = {-1, lightColor, darkColor};

    // gcc just sets all values manually :)

    for (unsigned type = 0; type < 3; type++) {
        for (unsigned y = 0; y < CELL_SIZE; y++) {
            for (unsigned x = 0; x < CELL_SIZE; x++) {
                const unsigned start = (type+1)*CELL_PIXELS + y*CELL_RENDER_SIZE*CELL_SCALE + x*CELL_SCALE;
                const uint16_t color = palette[cell_templates[type*CELL_SIZE*CELL_SIZE + y*CELL_SIZE + x]];

                cell_atlas[start                       ] = color;
                cell_atlas[start+1                     ] = color;
                cell_atlas[start+2                     ] = color;
                cell_atlas[start   +   CELL_RENDER_SIZE] = color;
                cell_atlas[start+1 +   CELL_RENDER_SIZE] = color;
                cell_atlas[start+2 +   CELL_RENDER_SIZE] = color;
                cell_atlas[start   + 2*CELL_RENDER_SIZE] = color;
                cell_atlas[start+1 + 2*CELL_RENDER_SIZE] = color;
                cell_atlas[start+2 + 2*CELL_RENDER_SIZE] = color;
            }
        }
    }
}

void cell_render(const unsigned type, const unsigned gridX, const unsigned gridY) {
    const unsigned x = gridX * (CELL_SIZE + 1) * CELL_SCALE;
    const unsigned y = gridY * (CELL_SIZE + 1) * CELL_SCALE;

    for (unsigned dy = 0; dy < CELL_RENDER_SIZE; dy++) {
        cp_copyMemory(
            &cell_atlas[type * CELL_PIXELS + dy * CELL_RENDER_SIZE],
            CELL_RENDER_SIZE * sizeof(uint16_t),
            &cp_pixels[(y + dy) * cp_getRenderWidth() + x]
        );
    }
}

#endif
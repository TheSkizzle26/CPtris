#ifndef BOARD
#define BOARD

#include <stdbool.h>
#include <stdint.h>

// ------------ INTERFACE -------------

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_CELLS (BOARD_WIDTH*BOARD_HEIGHT)

// TODO: check if other data types is faster
unsigned board_cells[BOARD_CELLS];
bool board_dirty[BOARD_CELLS];

void board_init();
void board_renderCell(unsigned i);
void board_renderDirty();

// ---------- IMPLEMENTATION ----------

void board_init() {
    for (unsigned i = 0; i < BOARD_CELLS; i++) {
        board_cells[i] = i % 2;
        board_dirty[i] = i % 3;
    }
}

void board_renderCell(const unsigned i) {
    
}

void board_renderDirty() {
    for (unsigned i = 0; i < BOARD_CELLS; i++) {
        if (board_dirty[i]) {
            board_dirty[i] = false;

        }
    }
}

#endif
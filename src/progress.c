#ifndef PROGRESS
#define PROGRESS

#include <stdint.h>

#include "cell.c"

// ------------ INTERFACE -------------

unsigned progress_lineScores[5] = {
    0,
    40,
    100,
    300,
    1200
};

uint16_t progress_levelPalettes[20] = {
    // 0
    CELL_COLOR(100, 176, 255),
    CELL_COLOR(66, 64, 255),
    // 1
    CELL_COLOR(136, 216, 0),
    CELL_COLOR(12, 147, 0),
    // 2
    CELL_COLOR(243, 106, 255),
    CELL_COLOR(160, 26, 204),
    // 3
    CELL_COLOR(92, 228, 48),
    CELL_COLOR(66, 64, 255),
    // 4
    CELL_COLOR(69, 224, 130),
    CELL_COLOR(183, 30, 123),
    // 5
    CELL_COLOR(146, 144, 255),
    CELL_COLOR(69, 224, 130),
    // 6
    CELL_COLOR(102, 102, 102),
    CELL_COLOR(181, 49, 32),
    // 7
    CELL_COLOR(110, 0, 64),
    CELL_COLOR(117, 39, 254),
    // 8
    CELL_COLOR(181, 49, 32),
    CELL_COLOR(66, 64, 255),
    // 9
    CELL_COLOR(234, 158, 34),
    CELL_COLOR(181, 49, 32),
};

unsigned progress_score;
unsigned progress_level;
unsigned progress_levelLines;

void progress_reset();
void progress_loadLevel(unsigned level);
void progress_nextLevel();
void progress_registerLineClears(unsigned lines);

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

#include "main.c"
#include "board.c"
#include "active.c"

void progress_reset() {
    progress_score = 0;
    progress_level = 0;
    progress_levelLines = 0;

    progress_loadLevel(13);
    board_reset();
    active_nextPiece();
}

void progress_loadLevel(const unsigned level) {
    progress_level = level;

    const unsigned levelMod = level % 10;
    cell_generateAtlas(
        progress_levelPalettes[levelMod*2    ],
        progress_levelPalettes[levelMod*2 + 1]
    );

    // fully re-render
    cp_setMemory(true, BOARD_CELLS, board_dirty);
    main_queryRefresh();
}

void progress_nextLevel() {
    progress_loadLevel(progress_level + 1);
}

void progress_registerLineClears(const unsigned lines) {
    progress_score += progress_lineScores[lines] * (progress_level + 1);
    progress_levelLines += lines;

    if (progress_levelLines >= 10) {
        progress_levelLines %= 10;
        progress_nextLevel();
    }
}

#endif

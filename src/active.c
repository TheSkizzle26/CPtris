#ifndef ACTIVE
#define ACTIVE

#include <stdbool.h>

// ------------ INTERFACE -------------

struct {
    bool changed;
    unsigned x;
    unsigned y;
    unsigned rotation;
    unsigned gravityTicks;
    unsigned piece;
    unsigned cellType;
    unsigned size;
    unsigned rotationCount;
    bool *rotations;
} active_current;

struct {
    unsigned leftTicks;
    unsigned rightTicks;
    bool moveLeft;
    bool moveRight;
} active_das;

unsigned active_stallTicks;
unsigned active_lastPiece;
unsigned active_hash = 5381;

bool active_templateCells[7][64] = {
    { // T
        0, 0, 0, 0,
        1, 1, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        1, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        0, 1, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // J
        0, 0, 0, 0,
        1, 1, 1, 0,
        0, 0, 1, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        0, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,

        1, 0, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        0, 1, 1, 0,
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // Z
        0, 0, 0, 0,
        1, 1, 0, 0,
        0, 1, 1, 0,
        0, 0, 0, 0,

        0, 0, 1, 0,
        0, 1, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // O
        1, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    },
    { // S
        0, 0, 0, 0,
        0, 1, 1, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,

        1, 0, 0, 0,
        1, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // L
        0, 0, 0, 0,
        1, 1, 1, 0,
        1, 0, 0, 0,
        0, 0, 0, 0,

        1, 1, 0, 0,
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,

        0, 0, 1, 0,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 1, 1, 0,
        0, 0, 0, 0,
    },
    { // I
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 1, 1, 1,
        0, 0, 0, 0,

        0, 0, 1, 0,
        0, 0, 1, 0,
        0, 0, 1, 0,
        0, 0, 1, 0,
    }
};

unsigned active_templateRotations[7] = {
    4, // T
    4, // J
    2, // Z
    1, // O
    2, // S
    4, // L
    2  // I
};

unsigned active_templateSizes[7] = {
    3, // T
    3, // J
    3, // Z
    2, // O
    3, // S
    3, // L
    4  // I
};

unsigned active_templateCellTypes[7] = {
    3, // T
    2, // J
    1, // Z
    3, // O
    2, // S
    1, // L
    3  // I
};

unsigned active_framesPerGridcell[30] = {
    48, 43, 38, 33, 28,
    23, 18, 13, 8,  6,
    5,  5,  5,  4,  4,
    4,  3,  3,  3,  2,
    2,  2,  2,  2,  2,
    2,  2,  2,  2,  1
};

unsigned active_AREDelay[] = {
    0, 0, // invisible
    18, 18, 18, 18,
    16, 16, 16, 16,
    14, 14, 14, 14,
    12, 12, 12, 12,
    10, 10, 10, 10
};

unsigned active_getFramesPerGridcell();
void active_stall(unsigned ticks);
void active_nextHash();
void active_nextPiece();
void active_markDirty();
bool active_isColliding();
void active_place();
void active_fall();
void active_attemptMove(signed movement);
void active_attemptRotation(const signed rotation);
void active_DAS();
void active_notStalled();
void active_tick();
void active_render();

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

#include "main.c"
#include "input.c"
#include "cell.c"
#include "board.c"
#include "progress.c"

unsigned active_getFramesPerGridcell() {
    return active_framesPerGridcell[progress_level > 29 ? 29 : progress_level];
}

void active_stall(const unsigned ticks) {
    active_stallTicks = ticks;
}

void active_nextHash() {
    active_hash = (active_hash << 5) + active_hash + cp_getTick() % 128;
}

void active_nextPiece() {
    unsigned piece = active_hash % 8;
    if ((piece == active_lastPiece) || (piece == 7 /* dummy */)) {
        active_nextHash();
        piece = active_hash % 7;
    }

    active_lastPiece = active_current.piece;
    
    const unsigned size = active_templateSizes[piece];

    active_current.changed = true;
    active_current.x = (BOARD_WIDTH - size) >> 1;
    active_current.y = size - 2;
    active_current.rotation = 0;
    active_current.gravityTicks = active_getFramesPerGridcell();
    active_current.piece = piece;
    active_current.cellType = active_templateCellTypes[piece];
    active_current.size = size;
    active_current.rotationCount = active_templateRotations[piece];
    active_current.rotations = active_templateCells[piece];
}

void active_markDirty() {
    if (!active_current.rotations)
        return;

    const unsigned rotationOffset = active_current.rotation * 16;

    for (unsigned dy = 0; dy < active_current.size; dy++)
        for (unsigned dx = 0; dx < active_current.size; dx++)
            if (active_current.rotations[rotationOffset + dy*4 + dx])
                board_markDirty(active_current.x + dx, active_current.y + dy);
}

bool active_isColliding() {
    const unsigned rotationOffset = active_current.rotation * 16;

    for (unsigned dy = 0; dy < active_current.size; dy++) {
        for (unsigned dx = 0; dx < active_current.size; dx++) {
            if (active_current.rotations[rotationOffset + dy*4 + dx]) {
                const unsigned gx = active_current.x + dx;
                const unsigned gy = active_current.y + dy;

                if (gx >= BOARD_WIDTH || gy >= BOARD_HEIGHT)
                    return true;

                if (board_getCell(gx, gy))
                    return true;
            }
        }
    }

    return false;
}

void active_place() {
    const unsigned rotationOffset = active_current.rotation * 16;
    unsigned dirtyCount = 0;
    unsigned clearCount = 0;
    unsigned pieceBottom = 0;

    for (unsigned dy = 0; dy < active_current.size; dy++) {
        const unsigned gy = active_current.y + dy;
        if (gy >= BOARD_HEIGHT) continue;

        for (unsigned dx = 0; dx < active_current.size; dx++) {
            if (active_current.rotations[rotationOffset + dy*4 + dx]) {
                board_setCell(active_current.x + dx, gy, active_current.cellType);

                if (gy > pieceBottom)
                    pieceBottom = gy;
            }
        }

        bool full = true;

        for (unsigned x = 0; x < BOARD_WIDTH; x++) {
            if (!board_getCell(x, gy)) {
                full = false;
                break;
            }
        }

        if (full) {
            clearCount++;

            if (gy-1 > dirtyCount)
                dirtyCount = gy-1;

            for (unsigned y = gy; y > 0; y--) {
                cp_copyMemory(
                    board_cells + (y-1) * BOARD_WIDTH,
                    BOARD_WIDTH * sizeof(unsigned),
                    board_cells + y * BOARD_WIDTH
                );
            }
        }
    }

    if (clearCount) {
        cp_setMemory(1, dirtyCount * BOARD_WIDTH, board_dirty + BOARD_WIDTH*2);
        progress_registerLineClears(clearCount);
        active_current.changed = true;
    }

    active_stall(active_AREDelay[pieceBottom]);
}

void active_fall() {
    active_current.gravityTicks--;

    if (!active_current.gravityTicks) {
        active_current.gravityTicks = active_getFramesPerGridcell();
        
        active_markDirty();
        
        active_current.y++;
        active_current.changed = true;

        if (active_isColliding()) {
            active_current.y--;
            active_place();
            // next piece is spawned after ARE
        }
    }
}

void active_attemptMove(const signed movement) {
    const unsigned old = active_current.x;
    const unsigned new = active_current.x + movement;

    active_current.x = new;
    const bool isColliding = active_isColliding();
    active_current.x = old;

    if (isColliding)
        return;

    active_markDirty();
    active_current.x = new;
    active_current.changed = true;
}

void active_attemptRotation(const signed rotation) {
    const unsigned old = active_current.rotation;
    const unsigned new = (active_current.rotation + rotation) % active_current.rotationCount;

    active_current.rotation = new;
    const bool isColliding = active_isColliding();
    active_current.rotation = old;

    if (isColliding)
        return;

    active_markDirty();
    active_current.rotation = new;
    active_current.changed = true;
}

void active_DAS() {
    active_das.moveLeft = false;
    active_das.moveRight = false;

    if (input_current.left) {
        active_das.leftTicks++;
        if (active_das.leftTicks == 16) {
            active_das.moveLeft = true;
            active_das.leftTicks = 10;
        }
    } else active_das.leftTicks = 0;

    if (input_current.right) {
        active_das.rightTicks++;
        if (active_das.rightTicks == 16) {
            active_das.moveRight = true;
            active_das.rightTicks = 10;
        }
    } else active_das.rightTicks = 0;
}

void active_notStalled() {
    const signed rotation = (
        (input_current.a && !input_last.a) -
        (input_current.b && !input_last.b)
    );

    if (rotation)
        active_attemptRotation(rotation);

    const signed movement = (
        (input_current.right && !input_last.right) -
        (input_current.left && !input_last.left)
    ) + (
        active_das.moveRight -
        active_das.moveLeft
    );

    if (movement)
        active_attemptMove(movement);

    active_fall();
}

void active_tick() {
    active_nextHash();
    active_DAS();

    if (active_stallTicks) {
        active_stallTicks--;

        if (!active_stallTicks)
            active_nextPiece();
    } else {
        active_notStalled();
    }
}

void active_render() {
    if (!active_current.changed)
        return;

    const unsigned rotationOffset = active_current.rotation * 16;

    for (unsigned dy = active_current.y < 2 ? 2 - active_current.y : 0; dy < active_current.size; dy++)
        for (unsigned dx = 0; dx < active_current.size; dx++)
            if (active_current.rotations[rotationOffset + dy*4 + dx])
                cell_render(active_current.cellType, active_current.x + dx, active_current.y + dy);

    active_current.changed = false;
    main_queryRefresh();
}

#endif
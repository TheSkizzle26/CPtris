#ifndef ACTIVE
#define ACTIVE

#include <stdbool.h>
#include <cp_base.c>

// ------------ INTERFACE -------------

struct {
    unsigned piece;
    unsigned size;
    unsigned rotations[64];
} active_current;

unsigned active_lastPiece;
unsigned active_hash = 5381;

struct {
    bool next;
} active_lastInputs;

void active_nextHash();
void active_nextPiece();
void active_generateRotations();
void active_update();
void active_render();

// ---------- IMPLEMENTATION ----------

#include <stdio.h>

void active_nextHash() {
    active_hash = (active_hash << 5) + active_hash + cp_getTick() % 128;
}

void active_nextPiece() {
    unsigned piece = active_hash % 7;

    if (piece == active_lastPiece) {
        active_nextHash();
        piece = active_hash % 7;
    }

    active_lastPiece = active_current.piece;

    printf("Piece: %u\n", piece);
    fflush(stdout);
}

void active_generateRotations() {

}

void active_update() {
    active_nextHash();

    if (cp_isKeyDown(CP_KEY_EXE))
        active_nextPiece();

    active_lastInputs.next = cp_isKeyDown(CP_KEY_EXE);
}

void active_render() {
    
}

#endif
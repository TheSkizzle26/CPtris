#ifndef INPUT
#define INPUT

#include <stdbool.h>

// ------------ INTERFACE -------------

typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    bool a;
    bool b;
} input_Keys;

input_Keys input_current;
input_Keys input_last;

void input_tick();

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

void input_tick() {
    // don't copy directly to avoid memcpy call for some reason?!?
    input_last.up = input_current.up;
    input_last.down = input_current.down;
    input_last.left = input_current.left;
    input_last.right = input_current.right;
    input_last.a = input_current.a;
    input_last.b = input_current.b;

    input_current.up = false;
    input_current.down = false;
    input_current.left = cp_isKeyDown(CP_KEY_7);
    input_current.right = cp_isKeyDown(CP_KEY_8);
    input_current.a = cp_isKeyDown(CP_KEY_EXE);
    input_current.b = cp_isKeyDown(CP_KEY_EXP);
}

#endif
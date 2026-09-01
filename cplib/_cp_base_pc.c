#ifndef CPLIB_BASE_SYSTEM
#define CPLIB_BASE_SYSTEM

#include <raylib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cp_base.c>

typedef enum : uint32_t {
    CP_KEY_SHIFT           = KEY_F2,
    CP_KEY_CLEAR           = KEY_F11,
    CP_KEY_BACKSPACE       = KEY_F12,
    CP_KEY_LEFT            = KEY_LEFT,
    CP_KEY_RIGHT           = KEY_RIGHT,
    CP_KEY_Z               = KEY_F8,
    CP_KEY_POWER           = KEY_F9,
    CP_KEY_DIVIDE          = KEY_F10,
    CP_KEY_MULTIPLY        = KEY_FIVE,
    CP_KEY_SUBTRACT        = KEY_T,
    CP_KEY_ADD             = KEY_G,
    CP_KEY_EXE             = KEY_B,
    CP_KEY_EXP             = KEY_V,
    CP_KEY_3               = KEY_F,
    CP_KEY_6               = KEY_R,
    CP_KEY_9               = KEY_FOUR,
    CP_KEY_KEYBOARD        = KEY_F1,
    CP_KEY_UP              = KEY_UP,
    CP_KEY_DOWN            = KEY_DOWN,
    CP_KEY_EQUALS          = KEY_F3,
    CP_KEY_X               = KEY_F4,
    CP_KEY_Y               = KEY_F5,
    CP_KEY_LEFT_BRACKET    = KEY_ONE,
    CP_KEY_RIGHT_BRACKET   = KEY_Q,
    CP_KEY_COMMA           = KEY_A,
    CP_KEY_NEGATIVE        = KEY_Z,
    CP_KEY_0               = KEY_X,
    CP_KEY_DOT             = KEY_C,
    CP_KEY_1               = KEY_S,
    CP_KEY_2               = KEY_D,
    CP_KEY_4               = KEY_W,
    CP_KEY_5               = KEY_E,
    CP_KEY_7               = KEY_TWO,
    CP_KEY_8               = KEY_THREE
} cp_Key;

Color *cp_pc_pixels;
Texture cp_pc_texture;

void *cp_allocate(const size_t size) {
    return malloc(size);
}

void cp_free(void *pointer) {
    free(pointer);
}

void cp_copyMemory(void *source, const size_t size, void *destination) {
    memcpy(destination, source, size);
}

void cp_init() {
    InitWindow(cp_getRenderWidth(), cp_getRenderHeight(), "");

    cp_pixels = cp_allocate(cp_getPixelCount() * sizeof(uint16_t));
    cp_pc_pixels = cp_allocate(cp_getPixelCount() * sizeof(Color));

    Image image = GenImageColor(cp_getRenderWidth(), cp_getRenderHeight(), BLACK);
    cp_pc_texture = LoadTextureFromImage(image);
    UnloadImage(image);
    
    cp_setTargetFPS(0);
}

void cp_quit() {
    CloseWindow();
}

void cp_setTargetFPS(const int fps) {
    SetTargetFPS(fps);
}

void cp_refreshScreen() {
    for (int i = 0; i < cp_getPixelCount(); i++) {
        const uint16_t color = cp_pixels[i];
        cp_pc_pixels[i].r = (color >> 8);
        cp_pc_pixels[i].g = (color >> 3) & 0b11111100;
        cp_pc_pixels[i].b = (color << 3) & 0b11111000;
        cp_pc_pixels[i].a = 255;
    }

    UpdateTexture(cp_pc_texture, cp_pc_pixels);
}

void cp_finishFrame() {
    BeginDrawing();
    DrawTexture(cp_pc_texture, 0, 0, WHITE);
    EndDrawing();

    cp_tick++;
}

bool cp_isKeyDown(const uint32_t key) {
    return IsKeyDown(key);
}

#endif
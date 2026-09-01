#ifndef CPLIB_BASE_SYSTEM
#define CPLIB_BASE_SYSTEM

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <cp_base.c>
#include <calc/cmt.c>
#include <calc/power.c>

#define cp_calc_malloc ((void *(*)(uint32_t))0x800CFB00)
#define cp_calc_free ((void (*)(void*))0x800A76FC)
#define cp_calc_memcpy ((void *(*)(void*, const void*, int))0x800A78AC)
#define cp_calc_fopen ((int (*)(char*, int))0x80057854)
#define cp_calc_fclose ((int (*)(int))0x80057912)
#define cp_calc_fread ((int (*)(int, void*, int))0x800578A2)
#define cp_calc_fseek ((int (*)(int, int, int))0x80057A96)
#define cp_calc_getVRAMAddress ((uint16_t *(*)())0x8002E154)
#define cp_calc_refreshLCD ((void (*)())0x8003733E)
#define cp_calc_VRAMBackup ((void (*)())0x8002D3FA)
#define cp_calc_VRAMRestore ((void (*)())0x8002D41A)

extern void cp_calc_getKey(uint32_t *key1, uint32_t *key2);

typedef enum : uint32_t {
    CP_KEY_SHIFT           = 0x80000000,
    CP_KEY_CLEAR           = 0x00020000,
    CP_KEY_BACKSPACE       = 0x00000080,
    CP_KEY_LEFT            = 0x00004000,
    CP_KEY_RIGHT           = 0x00008000,
    CP_KEY_Z               = 0x00002000,
    CP_KEY_POWER           = 0x00000040,
    CP_KEY_DIVIDE          = 0x40000000,
    CP_KEY_MULTIPLY        = 0x20000000,
    CP_KEY_SUBTRACT        = 0x10000000,
    CP_KEY_ADD             = 0x08000000,
    CP_KEY_EXE             = 0x04000000,
    CP_KEY_EXP             = 0x00000004,
    CP_KEY_3               = 0x00000008,
    CP_KEY_6               = 0x00000010,
    CP_KEY_9               = 0x00000020,
    CP_KEY_KEYBOARD        = 0x80000000,
    CP_KEY_UP              = 0x00800000,
    CP_KEY_DOWN            = 0x00400000,
    CP_KEY_EQUALS          = 0x00000080,
    CP_KEY_X               = 0x00000040,
    CP_KEY_Y               = 0x40000000,
    CP_KEY_LEFT_BRACKET    = 0x00000020,
    CP_KEY_RIGHT_BRACKET   = 0x00000010,
    CP_KEY_COMMA           = 0x00000008,
    CP_KEY_NEGATIVE        = 0x00000004,
    CP_KEY_0               = 0x04000000,
    CP_KEY_DOT             = 0x00040000,
    CP_KEY_1               = 0x08000000,
    CP_KEY_2               = 0x00080000,
    CP_KEY_4               = 0x10000000,
    CP_KEY_5               = 0x00100000,
    CP_KEY_7               = 0x20000000,
    CP_KEY_8               = 0x00200000
} cp_Key;

bool calc_pc_isFPSUnlocked = false;

void *cp_allocate(const size_t size) {
    return cp_calc_malloc(size);
}

void cp_free(void *pointer) {
    cp_calc_free(pointer);
}

void cp_copyMemory(void *source, const size_t size, void *destination) {
    cp_calc_memcpy(destination, source, size);
}

void cp_init() {
    cp_calc_VRAMBackup();
    cp_pixels = cp_calc_getVRAMAddress();

    cp_setTargetFPS(0);
}

void cp_quit() {
    cp_calc_VRAMRestore();
    cp_calc_refreshLCD();
}

void cp_setTargetFPS(const int fps) {
    if (fps <= 0) {
        cmt_stop();
        POWER_DISABLE->compareMatchTimer = 1;
        calc_pc_isFPSUnlocked = true;
    } else {
        POWER_DISABLE->compareMatchTimer = 0;
        cmt_set(CMT_TICKS_PER_SECOND / fps, CMT_MATCH_MODE_ONE_SHOT, CMT_REQUEST_DISABLE);
        cmt_start();
        calc_pc_isFPSUnlocked = false;
    }
}

void cp_refreshScreen() {
    cp_calc_refreshLCD();
}

void cp_finishFrame() {
    if (!calc_pc_isFPSUnlocked) {
        cmt_wait();
        cmt_start();
    }

    cp_tick++;
}

bool cp_isKeyDown(const uint32_t key) {
    uint32_t key1, key2;
    cp_calc_getKey(&key1, &key2);

    return (key1 == key) || (key2 == key);
}

#endif
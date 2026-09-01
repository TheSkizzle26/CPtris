#ifndef POWER
#define POWER

#include <stdint.h>

// ------------ INTERFACE -------------

typedef union {
    struct {
        uint32_t translationLookasideBuffer     : 1;
        uint32_t instructionCache               : 1;
        uint32_t operandCache                   : 1;
        uint32_t _reserved0                     : 1;
        uint32_t instructionLocalMemory         : 1;
        uint32_t _reserved1                     : 2;
        uint32_t floatingPointUnit              : 1;
        uint32_t _reserved2                     : 1;
        uint32_t interruptController            : 1;
        uint32_t directMemoryAccessController   : 1;
        uint32_t _reserved3                     : 1;
        uint32_t userDebugInterface             : 1;
        uint32_t debug                          : 1;
        uint32_t userBreakController            : 1;
        uint32_t superHyway                     : 1;
        uint32_t timerUnit                      : 1;
        uint32_t compareMatchTimer              : 1;
        uint32_t rewritableWatchdogTimer        : 1;
        uint32_t _reserved4                     : 3;
        uint32_t serialCommunication4           : 1;
        uint32_t serialCommunication5           : 1;
        uint32_t serialCommunication0           : 1;
        uint32_t serialCommunication1           : 1;
        uint32_t serialCommunication2           : 1;
        uint32_t serialCommunication3           : 1;
        uint32_t _reserved5                     : 1;
        uint32_t serialIO                       : 1;
        uint32_t _reserved6                     : 2;
    };

    uint32_t raw;
} power_Disable;

#define POWER_DISABLE ((volatile power_Disable *)0xA4150030)

#endif
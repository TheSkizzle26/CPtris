#ifndef DMAC
#define DMAC

#include <stdint.h>
#include <stdbool.h>
#include <cp_base.c>

// ------------ INTERFACE -------------

typedef enum {
    CMS_NORMAL          = 0x0,
    INTERMITTENT_16     = 0x2,
    INTERMITTENT_64     = 0x3,
    INTERMITTENT_256    = 0x4
} dmac_CycleStealMode;

typedef enum {
    PRIORITY_0  = 0x0, // CH0 > CH1 > CH2 > CH3 > CH4 > CH5
    PRIORITY_1  = 0x1, // CH0 > CH2 > CH3 > CH1 > CH4 > CH5
    ROUND_ROBIN = 0x3
} dmac_PriorityMode;

typedef enum {
    REPEAT_NORMAL       = 0x0,
    REPEAT_SAR_DAR_TCR  = 0x1,
    REPEAT_DAR_TCR      = 0x2,
    REPEAT_SAR_TCR      = 0x3,
    RELOAD_SAR_DAR_TCR  = 0x5,
    RELOAD_DAR_TCR      = 0x6,
    RELOAD_SAR_TCR      = 0x7
} dmac_RenewalMode;

typedef enum {
    READ_CYCLE  = 0x0,
    WRITE_CYCLE = 0x1
} dmac_AcknowledgeMode;

typedef enum {
    ACTIVE_LOW  = 0x0,
    ACTIVE_HIGH = 0x1
} dmac_AcknowledgeLevel;

typedef enum {
    SIZE_1_0    = 0x0,
    SIZE_2_0    = 0x1,
    SIZE_4_0    = 0x2,
    SIZE_16_0   = 0x3,
    SIZE_32_0   = 0x0,
    SIZE_8_0    = 0x3,
    SIZE_8x2_0  = 0x3,
    SIZE_16x2_0 = 0x0
} dmac_TransferSize0;

typedef enum {
    SIZE_1_1    = 0x0,
    SIZE_2_1    = 0x0,
    SIZE_4_1    = 0x0,
    SIZE_16_1   = 0x0,
    SIZE_32_1   = 0x1,
    SIZE_8_1    = 0x1,
    SIZE_8x2_1  = 0x2,
    SIZE_16x2_1 = 0x3
} dmac_TransferSize1;

typedef enum {
    DAR_FIXED_SOFT  = 0x0, // Address is fixed, but will be incremented in 16/32-byte division
                           // transfer mode. Check SH7730 User's Manual Figure 12.3.7
    DAR_INCREMENT   = 0x1,
    DAR_DECREMENT   = 0x2, // Prohibited in 8/16/32-byte transfer mode
    DAR_FIXED_HARD  = 0x3  // Address is fixed, even in 16/32-byte division transfer mode.
} dmac_DestinationMode;

typedef enum {
    SAR_FIXED_SOFT  = 0x0, // Address is fixed, but will be incremented in 16/32-byte division  
                           // transfer mode. Check SH7730 User's Manual Figure 12.3.7
    SAR_INCREMENT   = 0x1,
    SAR_DECREMENT   = 0x2, // Prohibited in 8/16/32-byte transfer mode
    SAR_FIXED_HARD  = 0x3  // Address is fixed, even in 16/32-byte division transfer mode.
} dmac_SourceMode;

typedef enum {
    EXTERNAL  = 0x0,
    AUTO      = 0x4,
    DMARS     = 0x8
} dmac_ResourceSelect;

typedef enum {
    LOW_LEVEL     = 0x0,
    FALLING_EDGE  = 0x1,
    HIGH_LEVEL    = 0x2,
    RISING_EDGE   = 0x3
} dmac_LevelEdgeSelect;

typedef enum {
    CYCLE_STEAL = 0x0,
    BURST       = 0x1
} dmac_TransferBusMode;

typedef union {
    struct {
        uint32_t                _reserved0              : 1;
        uint32_t                busRelease              : 1; // bus release enable in cycle steal mode
        uint32_t                _reserved1              : 2;
        dmac_RenewalMode        renewalMode             : 3; // DMA settings renewal specify
        uint32_t                _reserved2              : 1;
        uint32_t                overrun                 : 1;
        uint32_t                _reserved3              : 1;
        dmac_TransferSize1      transferSize1           : 2; // DMA transfer size specify (MS 2 bits)
        uint32_t                halfEnd                 : 1;
        uint32_t                halfEndInterruptEnable  : 1;
        dmac_AcknowledgeMode    acknowledgeMode         : 1;
        dmac_AcknowledgeLevel   acknowledgeLevel        : 1;
        dmac_DestinationMode    destinationMode         : 2; // destination address mode
        dmac_SourceMode         sourceMode              : 2; // source address mode
        dmac_ResourceSelect     resourceSelect          : 4;
        dmac_LevelEdgeSelect    levelEdgeSelect         : 2; // DREQ level and edge select
        dmac_TransferBusMode    transferBusMode         : 1;
        dmac_TransferSize0      transferSize0           : 2; // DMA transfer size specify (LS 2 bits)
        uint32_t                interruptEnable         : 1; // interrupt enable
        uint32_t                transferEnd             : 1; // transfer end flag
        uint32_t                enabled                 : 1; // DMA enable
    };

    uint32_t raw;
} dmac_ChannelControlRegister;

typedef union {
    struct {
        dmac_CycleStealMode     cycleStealMode      : 4;
        uint16_t                _reserved0          : 2;
        dmac_PriorityMode       priorityMode        : 2;
        uint16_t                _reserved1          : 5;
        uint16_t                addressErrorFlag    : 1;
        uint16_t                NMIFlag             : 1;
        uint16_t                masterEnabled       : 1; // DMA master enable
    };

    uint16_t raw;
} dmac_CommonRegister;

// Channel 0
#define DMAC_SAR_0  ((volatile uint32_t *)0xFE008020)
#define DMAC_DAR_0  ((volatile uint32_t *)0xFE008024)
#define DMAC_TCR_0  ((volatile uint32_t *)0xFE008028)
#define DMAC_CHCR_0 ((volatile dmac_ChannelControlRegister *)0xFE00802C)

// Channel 1
#define DMAC_SAR_1  ((volatile uint32_t *)0xFE008030)
#define DMAC_DAR_1  ((volatile uint32_t *)0xFE008034)
#define DMAC_TCR_1  ((volatile uint32_t *)0xFE008038)
#define DMAC_CHCR_1 ((volatile dmac_ChannelControlRegister *)0xFE00803C)

// Common
#define DMAC_DMAOR  ((volatile dmac_CommonRegister *)0xFE008060)

// Channel 0 B-Registers
#define DMAC_SARB_0 ((volatile uint32_t *)0xFE008120)
#define DMAC_DARB_0 ((volatile uint32_t *)0xFE008124)
#define DMAC_TCRB_0 ((volatile uint32_t *)0xFE008128)

// Channel 1 B-Registers
#define DMAC_SARB_1 ((volatile uint32_t *)0xFE008130)
#define DMAC_DARB_1 ((volatile uint32_t *)0xFE008134)
#define DMAC_TCRB_1 ((volatile uint32_t *)0xFE008138)

// ---------- IMPLEMENTATION ----------

/**
 * Waits for a DMA operation to complete on a channel.
 *   
 * @param chcr The control register of the channel to wait for.
 * 
 * @return Returns true on successful operation and false for an address error.
*/
bool dma_wait(volatile dmac_ChannelControlRegister *chcr) {
    // Check if DMA was never running
    if (cp_unlikely(!chcr->enabled || !DMAC_DMAOR->masterEnabled)) {
        return true;
    }

    while (true) {
        if (DMAC_DMAOR->addressErrorFlag) {
            // Address error
            DMAC_DMAOR->addressErrorFlag = 0;
            return false;
        }

        if (chcr->transferEnd) {
            // Success
            return true;
        }
    }
}

#endif
#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

#include <stdint.h>

// Define meaningful constants
#define GDT_ACCESS_PRESENT     0x80
#define GDT_ACCESS_RING0       0x00
#define GDT_ACCESS_RING3       0x60
#define GDT_ACCESS_CODE_DATA   0x10
#define GDT_ACCESS_EXECUTABLE  0x08
#define GDT_ACCESS_DIR_CONF    0x04
#define GDT_ACCESS_RW          0x02
#define GDT_ACCESS_ACCESSED    0x01

#define GDT_FLAG_GRANULARITY   0x08  // Bit 3: 0 = 1B blocks, 1 = 4KB blocks
#define GDT_FLAG_SIZE_32       0x04  // Bit 2: 0 = 16-bit, 1 = 32-bit
#define GDT_FLAG_LONG_MODE     0x02  // Bit 1: 1 = 64-bit (only for x86_64)
#define GDT_FLAG_RESERVED      0x01  // Bit 0: Always 0

#define GDT_BASE 0x00000000U
#define GDT_LIMIT 0xFFFFFFFFU
#define GDT_STARTING_ADDRESS 0x00000800
#define GDT_SIZE 24

typedef enum {
    KERNEL = 0,
    DRIVER_1 = 1,
    DRIVER_2 = 2,
    USER_SPACE = 3,
} Privilege;

typedef struct {
    uint16_t limit_low;       // Lower 16 bits of the limit
    uint16_t base_low;        // Lower 16 bits of the base
    uint8_t  base_middle;     // Next 8 bits of the base
    uint8_t access;          // Access byte (Privilege, Type, etc.)
    uint8_t  limit_high : 4;  // Last 4 bits of the limit
    uint8_t  flags      : 4;  // Flags (Granularity, Size, etc.)
    uint8_t  base_high;       // Last 8 bits of the base
} __attribute__((packed)) segment_descriptor;    // Prevents the compiler from adding padding

typedef struct {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) gdtr ;

// struct gdtr {
//     uint64_t base_address;
//     segment_descriptor* sd;
// };

void init_gdt(void);

#endif

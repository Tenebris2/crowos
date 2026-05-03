#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H

#include <stdint.h>

typedef enum {
    KERNEL = 0,
    DRIVER_1 = 1,
    DRIVER_2 = 2,
    USER_SPACE = 3,
} Privilege;

typedef struct {
    uint8_t accessed   : 1;
    uint8_t read_write : 1;
    uint8_t dir_conf   : 1;
    uint8_t executable : 1;
    uint8_t s_type     : 1; // 1 for code/data
    uint8_t dpl        : 2; // Privilege
    uint8_t present    : 1;
} access __attribute__((packed));

typedef struct {
    uint16_t limit_low;       // Lower 16 bits of the limit
    uint16_t base_low;        // Lower 16 bits of the base
    uint8_t  base_middle;     // Next 8 bits of the base
    access access;          // Access byte (Privilege, Type, etc.)
    uint8_t  limit_high : 4;  // Last 4 bits of the limit
    uint8_t  flags      : 4;  // Flags (Granularity, Size, etc.)
    uint8_t  base_high;       // Last 8 bits of the base
} segment_descriptor __attribute__((packed));    // Prevents the compiler from adding padding

// struct gdtr {
//     uint64_t base_address;
//     segment_descriptor* sd;
// };

void init_kernel_segment(void);

#endif

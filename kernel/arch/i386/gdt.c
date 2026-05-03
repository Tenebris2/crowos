#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/tty.h>

volatile uint64_t *BASE_ADDRESS = (volatile uint64_t *)0x00000000;

segment_descriptor gdt[3] __attribute__((section(".gdt_mem")));
gdtr gdt_register;

segment_descriptor get_flat_descriptor() {
  segment_descriptor sd = {0};
  sd.limit_low  = GDT_LIMIT & 0xFFFF;
  sd.limit_high = (GDT_LIMIT >> 16) & 0xF;
  sd.base_low    = GDT_BASE & 0xFFFF;
  sd.base_middle = (GDT_BASE >> 16) & 0xFF;
  sd.base_high   = (GDT_BASE >> 24) & 0xFF;
  return sd;
}

void init_kernel_code_segment(int index) {
  gdt[index] = get_flat_descriptor();
  gdt[index].flags = GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE_32;
  gdt[index].access = GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 |
                      GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE |
                      GDT_ACCESS_RW;
}
void init_kernel_data_segment(int index) {
  gdt[index] = get_flat_descriptor();

  // Flags remain the same: 4KB Granularity + 32-bit mode
  gdt[index].flags = GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE_32;

  // Access: Note the absence of EXECUTABLE
  gdt[index].access = GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 |
                      GDT_ACCESS_CODE_DATA |
                      GDT_ACCESS_RW; // For Data, RW means Writable
}

void init_gdtr() {
  gdt_register.size = (uint16_t) sizeof(gdt) - 1;
  gdt_register.offset = (uint32_t) 0x800;
}

void load_gdt() {
    // 1. Load the GDTR register
    asm volatile("lgdt %0" : : "m"(gdt_register));

    // // 2. Reload the segment registers (The part you have)
    asm volatile(
        "movw $0x10, %ax \n"    // 0x10 is Index 2 (Data)
        "movw %ax, %ds   \n"
        "movw %ax, %es   \n"
        "movw %ax, %fs   \n"
        "movw %ax, %gs   \n"
        "movw %ax, %ss   \n"    // Don't forget the Stack Segment!
        "ljmp $0x08, $1f \n" // 0x08 is Index 1 (Code)
        "1:          \n"
    );
}
// Usage is now much more readable
void init_gdt() {
  // Null segment
  *((uint64_t *)&gdt[0]) = 0; // set null descriptor
  // Kernel Code: Present | Ring0 | Code/Data | Executable | Readable
  init_kernel_code_segment(1);
  init_kernel_data_segment(2);

  init_gdtr();
  load_gdt();
}

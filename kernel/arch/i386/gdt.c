#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/gdt.h>

volatile uint64_t *BASE_ADDRESS = (volatile uint64_t *)0x00000000;

segment_descriptor gdt[3] __attribute__((section(".gdt_mem")));

segment_descriptor get_flat_descriptor() {
  segment_descriptor sd;
  sd.base_low = (GDT_BASE & 0xFFFF);
  sd.base_middle = (GDT_BASE >> 16) & 0xFF;
  sd.base_high = (GDT_BASE >> 24) & 0xFF;

  sd.limit_low = (GDT_LIMIT & 0xFFFF);
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

// Usage is now much more readable
void init_gdt() {
  // Null segment
  *((uint64_t *)&gdt[0]) = 0; // set null descriptor
  // Kernel Code: Present | Ring0 | Code/Data | Executable | Readable
  init_kernel_code_segment(1);
  init_kernel_data_segment(2);
}

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/gdt.h>

volatile uint64_t *BASE_ADDRESS = (volatile uint64_t *)0x00000000;

void set_kernel_segment_descriptor(int index,
                                   segment_descriptor *sd,
                                   uint8_t read_write,
                                   uint8_t dir_conf,
                                   uint8_t executable,
                                   uint8_t s_type,
                                   uint8_t priv_level,
                                   uint8_t flags) {
  sd[index].limit_low = 0xFFFF;
  sd[index].base_low = 0x0000;
  sd[index].base_middle = 0x00;
  sd[index].limit_high = 0xF;
  sd[index].flags = flags;
  sd[index].base_high = 0x00;

  sd[index].access.accessed = 1; // sets to 1 if the segment is in memory, else trigger "segment not present"
  sd[index].access.read_write = read_write; //
  sd[index].access.dir_conf = dir_conf;
  sd[index].access.executable = executable;
  sd[index].access.s_type = s_type; // 1 if it a system segment(tss or ldt), 0 if it is a code or data segment
  sd[index].access.dpl = priv_level; //descriptor privilege level, 0, 1, 2, 3; 0: kernel level, 1-2: driver, 3: userspace
  sd[index].access.present = 1;
}

void init_kernel_segment(void) {
  segment_descriptor sd[3];
  *((uint64_t *)&sd[0]) = UINT64_C(0);
  set_kernel_segment_descriptor(1, sd, 1, 0, 1, 1, 0, 0xC);
  set_kernel_segment_descriptor(2, sd, 1, 0, 0, 1, 0, 0xC);
}

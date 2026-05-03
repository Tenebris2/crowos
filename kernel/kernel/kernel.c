#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
  terminal_initialize();
  init_kernel_segment();
  printf("Hello, kernel World!\n");
}

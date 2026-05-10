#include <stdio.h>
#include <kernel/tty.h>

void exception_handler(int exception_num, int error_code) {
    printf("Exception %d - Error Code: 0x%x\n", exception_num, error_code);
    asm volatile ("cli");
    while(1) asm volatile ("hlt");
}

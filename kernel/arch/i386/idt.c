#include <kernel/gdt.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/idt.h>

#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0x8F

static isr_t idt[256];
static idtr_t idt_register;
extern void* isr_stub_table[];

void init_isr(int i) {
    uint32_t isr_address = (uint32_t)isr_stub_table[i];
    idt[i].attributes = INTERRUPT_GATE;
    idt[i].reserved = 0;
    idt[i].selector = 0x8;
    idt[i].offset_low = isr_address & 0xFFFF;
    idt[i].offset_high = (isr_address >> 16) & 0xFFFF;
}
void init_idtr() {
    idt_register.size = sizeof(idt)-1;
    idt_register.offset = (uint32_t)idt;
}

void load_idtr() {
    asm volatile("lidt %0" : : "m"(idt_register));
}

void init_idt() {
    memset(&idt, 0, sizeof(idt));
    for (int i = 0; i < 32; i++) {
        init_isr(i);
    }

    init_idtr();
    load_idtr();

    asm volatile("sti");
}

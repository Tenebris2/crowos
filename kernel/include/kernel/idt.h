#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H

#include <stdint.h>

typedef struct {
  uint16_t offset_low; // address of the isr
  uint16_t selector;   // the gdt segment selector(which selector to start
                       // executing in(the offset), for example: in our gdt then
                       // it would be 0x8 for the code segment)
  uint8_t reserved;
  uint8_t
      attributes; // MAGIC_NUMBER; [present_bit dpl reserved type(two types of
  // interrupts: interrupt_gate(1110)(cpu can't do anything
  // except handle this interrupt, needs an ack so that the cpu
  // can actually do thigns now), trap_gate(1111)(worry about this later when i
  // actually have a userspace))] 1 00 0
  uint16_t offset_high;
} __attribute__((packed)) isr_t; // interrupt service routine => 16bytes; 64bit

// Address of the isr, defined in an assembly file, extern those isr, convert into an unsigned long long(64bit value).
// Simply AND it.
// We need 256 interrupts ISR -> 2**4 * 2**8 = 4096 bytes.
// To load the idt, we have the idtr.
//
typedef struct {
    uint16_t size; // also actual_size - 1
    uint32_t offset;
} __attribute__((packed)) idtr_t;

void init_idt(void);
#endif

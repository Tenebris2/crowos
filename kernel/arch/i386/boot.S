# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text

.altmacro
.macro idt_stub index
    .global idt_stub_\index
    .type idt_stub_\index, @function
    idt_stub_\index:
        pushl $0
        pushl $\index
        jmp common_stub
.endm

.set i, 0
.rept 32
    idt_stub %i
    .set i, i + 1
.endr

.global common_stub
.type common_stub, @function

common_stub:
    .extern exception_handler
    # 1. Save general purpose registers
    pusha

    # 2. Save Segment Registers
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    # 3. Load Kernel Data Segment
    movl $0x10, %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    # 4. Call exception handler - push args explicitly (cdecl, right-to-left)
    # Stack at this point: gs,fs,es,ds (8B) + pusha (32B) = 40B above exception_num
    pushl 44(%esp)   # error_code (at esp+44; was esp+40 before first push shifts it)
    pushl 44(%esp)   # exception_num (now at esp+44 after above push)
    call exception_handler
    addl $8, %esp    # caller cleanup

    # 5. Restore State
    popw %gs
    popw %fs
    popw %es
    popw %ds
    popa

    # 6. Clean up error code and vector number
    addl $8, %esp

    iret

.global _start
.type _start, @function

_start:
	movl $stack_top, %esp
	# Hang if kernel_main unexpectedly returns.
	; cli
	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main
#     # Now the CPU is fully using the GDT at 0x800

1:	hlt
	jmp 1b
.size _start, . - _start

.section .data

.global isr_stub_table
isr_stub_table:
    .long idt_stub_0, idt_stub_1, idt_stub_2, idt_stub_3
    .long idt_stub_4, idt_stub_5, idt_stub_6, idt_stub_7
    .long idt_stub_8, idt_stub_9, idt_stub_10, idt_stub_11
    .long idt_stub_12, idt_stub_13, idt_stub_14, idt_stub_15
    .long idt_stub_16, idt_stub_17, idt_stub_18, idt_stub_19
    .long idt_stub_20, idt_stub_21, idt_stub_22, idt_stub_23
    .long idt_stub_24, idt_stub_25, idt_stub_26, idt_stub_27
    .long idt_stub_28, idt_stub_29, idt_stub_30, idt_stub_31

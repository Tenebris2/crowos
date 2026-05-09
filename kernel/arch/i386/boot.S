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
    .type init_stub_\index, @function
    idt_stub_\index:
        pushl $0
        pushl $\index
        jmp common_stub
        iret
.endm

.global common_stub
.type common_stub, @function

common_stub:
    .extern exception_handler
    # 1. Save general purpose registers
    # If in 32-bit mode, pusha saves EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    pusha

    # 2. Save Segment Registers
    # (DS is usually enough, but saving ES, FS, and GS is safer)
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    # 3. Load Kernel Data Segment
    # This ensures the kernel can actually access its own global variables
    movl $0x10, %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    # 4. Pass the stack pointer to C
    # The current ESP points to the start of our "struct" on the stack
    call exception_handler
    addl $4, %esp

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
.set i, 0
isr_stub_table:
    .rept 32
        idt_stub %i
        .set i, i + 1
    .endr

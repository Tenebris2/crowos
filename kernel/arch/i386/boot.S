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

.section .data
.align 2 # Ensure the whole struct starts on a 2-byte boundary

gdt_ptr:
    .word 0x17          # Limit: (3 entries * 8) - 1
    .long 0x800         # Base Address: 0x800

# The kernel entry point.
.section .text
.global _start
.type _start, @function

_start:
	movl $stack_top, %esp
	# Hang if kernel_main unexpectedly returns.
	cli
	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main
	lgdt gdt_ptr

    # 2. Perform a Far Jump to reload CS (Code Segment)
    # This is required to switch the CPU to the new GDT context
    ljmp $0x08, $.reload_cs

.reload_cs:
    # 3. Reload Data Segment registers
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Now the CPU is fully using the GDT at 0x800

1:	hlt
	jmp 1b
.size _start, . - _start

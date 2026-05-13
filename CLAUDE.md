# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

**Requires** an `i686-elf-gcc` cross-compiler on `$PATH`. The toolchain is configured in `config.sh`.

```sh
./build.sh          # Compile libc (libk.a) + kernel, install to sysroot/
./iso.sh            # build.sh + create crowos.iso via grub-mkrescue
./qemu.sh           # iso.sh + boot in QEMU
./clean.sh          # Clean build artifacts
```

**Debug** (requires two terminals):
```sh
# Terminal 1 — QEMU paused waiting for GDB
qemu-system-i386 -cdrom crowos.iso -no-reboot -d int -s -S

# Terminal 2 — GDB
./debug.sh          # connects to :1234, breaks at kernel_main
```

## Architecture

The repo has two subprojects built in order: `libc` then `kernel`.

### libc
Source lives in `libc/`. It builds **two** static archives:
- `libk.a` — the kernel-mode variant, compiled with `-D__is_libk`. No hosted OS assumptions.
- `libc.a` — not yet built (commented out in `libc/Makefile`).

Code that behaves differently between the two uses `#ifdef __is_libk`. Currently only `libk.a` is used.

### kernel
Entry point: `kernel/arch/i386/boot.S` → `_start` → `kernel_main` (`kernel/kernel/kernel.c`).

**Interrupt flow** (i386):
1. `boot.S` defines 32 ISR stubs (via `.macro idt_stub`) that push `(error_code=0, index)` then jump to `common_stub`.
2. `common_stub` saves all registers (`pusha` + segment `pushw`s), then explicitly pushes `exception_num` and `error_code` before calling the C handler.
3. `kernel/arch/i386/idt.c` → `init_idt()` populates the IDT entries for vectors 0–31 using `isr_stub_table[]` (defined at the bottom of `boot.S`).
4. `kernel/arch/i386/interrupt_handler.c` → `exception_handler(int exception_num, int error_code)` handles all exceptions (currently: print + halt).

**Key constraint**: The 8259 PIC is **not remapped**. Hardware IRQs (timer = IRQ 0) fire on vector 8, which collides with the Double Fault exception. Remap the PIC before enabling IRQ handlers.

**Calling convention in stubs**: x86 cdecl — caller pushes args before `call`. `common_stub` pushes args at `esp+44` (error_code) and `esp+44` (exception_num, after first push shifts it) to match what the C function expects.

### sysroot
`sysroot/` acts as a fake installation root. `build.sh` installs libc headers + `libk.a` there, then the kernel Makefile picks them up via `--sysroot`.

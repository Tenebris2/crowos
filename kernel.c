#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  (volatile uint16_t*) 0xB8000

uint16_t set_color(char str) {
    uint8_t colour = VGA_COLOR_BLACK << 4 | VGA_COLOR_BLUE;
    return colour << 8 | (uint8_t)str;
}

void write_char(char str, int row, int col) {
    int index = row * VGA_WIDTH + col;
    *(VGA_MEMORY+index) = set_color(str);
}

void write(char* str_ptr) {
    int position = 0;
    while (*str_ptr != '\0') {
        write_char(*str_ptr, position, 0);
        str_ptr++;
        position++;
    }
}

void kernel_main() {
    write("Hello World");
}

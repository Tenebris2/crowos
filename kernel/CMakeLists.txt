add_executable(crowos.kernel
    kernel/kernel.c
    arch/i386/interrupt_handler.c
    arch/i386/tty.c
    arch/i386/gdt.c
    arch/i386/idt.c
    arch/i386/boot.S
    arch/i386/crti.S
    arch/i386/crtn.S
)

target_compile_options(crowos.kernel PRIVATE
    -ffreestanding
    -Wall
    -Wextra
    -O2
    -g
)

target_compile_definitions(crowos.kernel PRIVATE
    __is_kernel
)

target_include_directories(crowos.kernel PRIVATE include)

target_link_libraries(crowos.kernel PRIVATE k)

set_target_properties(crowos.kernel PROPERTIES
    LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/arch/i386/linker.ld -ffreestanding -O2 -nostdlib -lgcc"
)

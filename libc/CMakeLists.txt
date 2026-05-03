add_library(k STATIC
    string/memcmp.c
    string/memcpy.c
    string/memmove.c
    string/memset.c
    string/strlen.c
    stdio/printf.c
    stdio/puts.c
    stdio/putchar.c
    stdlib/abort.c
)

target_compile_options(k PRIVATE
    -ffreestanding
    -Wall
    -Wextra
    -O2
    -g
)

target_compile_definitions(k PRIVATE
    __is_libk
)

target_include_directories(k PUBLIC include)

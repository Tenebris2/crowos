#!/bin/bash

gdb -ex "target remote :1234" -ex "break kernel_main" -ex "continue" ./sysroot/boot/crowos.kernel

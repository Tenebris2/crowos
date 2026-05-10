#!/bin/sh
set -e
. ./iso.sh

# qemu-system-$(./target-triplet-to-arch.sh $HOST) \
#     -cdrom crowos.iso -no-reboot -d int -s -S 2>&1 | head -50
qemu-system-$(./target-triplet-to-arch.sh $HOST) \
    -cdrom crowos.iso -no-reboot

#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/crowos.kernel isodir/boot/crowos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "crowos" {
	multiboot /boot/crowos.kernel
}
EOF
grub-mkrescue -o crowos.iso isodir

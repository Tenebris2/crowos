SHELL := /usr/bin/fish
# Define the kernel name
KERNEL = crowos
ISO = crowos.iso
# Define variables if not already set
BINARY_DIR ?= build

# A 'setup' target to ensure the directory exists
setup:
	mkdir -p $(BINARY_DIR)

$(ISO): $(KERNEL) grub.cfg
	@if grub-file --is-x86-multiboot $(KERNEL); then \
		echo "Multiboot confirmed"; \
	else \
		echo "Error: $(KERNEL) is not a valid multiboot file"; \
		exit 1; \
	fi
	# Proceed with ISO creation commands here
	mkdir -p isodir/boot/grub
	cp $(BINARY_DIR)/$(KERNEL) isodir/boot/$(KERNEL)
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir


build-boot: setup
	cross-as boot.s -o $(BINARY_DIR)/boot.o
build-code: setup
	cross -c kernel.c -o $(BINARY_DIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
link: build-code build-boot setup
	cross -T linker.ld -o $(BINARY_DIR)/$(KERNEL) -ffreestanding -O2 -nostdlib $(BINARY_DIR)/boot.o $(BINARY_DIR)/kernel.o -lgcc

build: link setup
	mkdir -p isodir/boot/grub
	cp $(BINARY_DIR)/$(KERNEL) isodir/boot/crowos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BINARY_DIR)
	rm -rf isodir
	rm $(ISO)

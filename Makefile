SHELL := /usr/bin/fish
# Define the kernel name
KERNEL = crowos
ISO = crowos.iso

$(ISO): $(KERNEL) grub.cfg
	@if grub-file --is-x86-multiboot $(KERNEL); then \
		echo "Multiboot confirmed"; \
	else \
		echo "Error: $(KERNEL) is not a valid multiboot file"; \
		exit 1; \
	fi
	# Proceed with ISO creation commands here
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/$(KERNEL)
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

build-boot:
	cross-as boot.s -o boot.o
build-code:
	cross -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
link: build-code build-boot
	cross -T linker.ld -o $(KERNEL) -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

build: link
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/crowos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm boot.o kernel.o $(KERNEL)
	rm -rf isodir
	rm $(ISO)

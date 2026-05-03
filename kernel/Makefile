DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)
BUILDDIR=build

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS:%.o=$(BUILDDIR)/%.o) \
$(BUILDDIR)/kernel/kernel.o \

OBJS=\
$(BUILDDIR)/$(ARCHDIR)/crti.o \
$(BUILDDIR)/$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(BUILDDIR)/$(ARCHDIR)/crtend.o \
$(BUILDDIR)/$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(BUILDDIR)/$(ARCHDIR)/crti.o \
$(BUILDDIR)/$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(BUILDDIR)/$(ARCHDIR)/crtend.o \
$(BUILDDIR)/$(ARCHDIR)/crtn.o \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

$(BUILDDIR) $(BUILDDIR)/$(ARCHDIR):
	mkdir -p $@

all: crowos.kernel

crowos.kernel: $(OBJS) $(ARCHDIR)/linker.ld | $(BUILDDIR)
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot crowos.kernel

$(BUILDDIR)/$(ARCHDIR)/crtbegin.o $(BUILDDIR)/$(ARCHDIR)/crtend.o: | $(BUILDDIR)/$(ARCHDIR)
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	@mkdir -p $(@D)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

$(BUILDDIR)/%.o: %.S | $(BUILDDIR)
	@mkdir -p $(@D)
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f crowos.kernel
	rm -rf $(BUILDDIR)

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: crowos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp crowos.kernel $(DESTDIR)$(BOOTDIR)

-include $(shell find $(BUILDDIR) -name '*.d' 2>/dev/null)

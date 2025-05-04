#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/myos.kernel isodir/boot/myos.kernel

cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
	multiboot /boot/myos.kernel
}
EOF

# ↓ minimal changes: force BIOS modules and disable the interactive CLI
grub-mkrescue \
	-d /usr/lib/grub/i386-pc \
	--install-modules="multiboot normal iso9660 part_msdos ext2 search search_fs_uuid linux configfile" \
	-o myos.iso isodir
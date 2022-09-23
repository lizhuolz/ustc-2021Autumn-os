.globl start

MAGIC_ITEM_NAME = 0x1BADB002 ;
FLAGS_ITEM_NAME = 0x00 ;
CHECKSUM_ITEM_NAME = - 0x1BADB002 ;

.section ".multiboot_header"

	.long  MAGIC_ITEM_NAME
	.long  FLAGS_ITEM_NAME
	.long  CHECKSUM_ITEM_NAME
	.long  start
.text
.code32
start:

	movl $0x2f452f48, 0xB8000
	movl $0x2f4c2f4c, 0xB8004
	movl $0x2f572f4f, 0xB8008
	movl $0x2f522f4f, 0xB800c
	movl $0x2f442f4c, 0xB8010
	movl $0x2f202f21, 0xB8014

	movl $0x2f422f50, 0xB8018
	movl $0x2f392f31, 0xB801c
	movl $0x2f302f30, 0xB8020
	movl $0x2f302f30, 0xB8024
	movl $0x2f342f36, 0xB8028

	movb $0x46, %al
	movw $0x3F8, %dx
	outb %al, %dx 
	nop
	nop
	hlt



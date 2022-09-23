.globl start

MAGIC_ITEM_NAME = 0x1BADB002 ;
FLAGS_ITEM_NAME = 0x00 ;
CHECKSUM_ITEM_NAME = - 0x1BADB002 ;

.section "multiboot_header"

	.long  MAGIC_ITEM_NAME
	.long  FLAGS_ITEM_NAME
	.long  CHECKSUM_ITEM_NAME
	.long  start
.text
.code32
start:
	movl $0x2f4b2f4f, 0xB8000
	movb $0x46, %al
	movw $0x3F8, %dx
	outb %al, %dx 
	nop
	nop

~
~
~
~

.section .text
.code16

  # disable interrupts
  cli

  # canonicalize %CS:%EIP to a known value
  ljmp $0, $start
start:

  # setup segments
  movw $0x0000, %ax
  movw %ax, %ss
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs

  # setup stack
  movw $0x9000, %sp
  movw %sp, %bp

  # select display page 0
  movb $0, %al
  movb $0x05, %ah
  int $0x10

  # print hello world
  movw $message, %si
  call print

  # TODO:
  # - activate A20 gate

  # reset bootdrive
reset:
  movb $0x00, %ah
  int $0x13
  jc reset

  # load second stage into memory starting at adress 0x10000
  #   (NOTE: this code can only be jumped to after loading the global descriptor table
  #          because it uses absolute adresses larger than 16 bit)
  #   (NOTE2: this routine only loads 0x48 sectors of the second stage into memory
  #          and is in general pretty whacky. should be replaced with sth more serious)
load:
  movw $0x0000, %bx   # buffer address
  movw $0x1000, %ax
  movw %ax, %es       # buffer address (segment)
  movb $0x02, %ah     # function 0x02: read a sector
  movb $0x48, %al     # sectors to read count
  movb $0x00, %ch     # cylinder
  movb $0x02, %cl     # sector
  movb $0x00, %dh     # head
  # dl (drive) keep as is
  int $0x13
  jnc load

  # - load global descriptor table and far jump into protected mode
  lgdt (GDT_DESCRIPTOR)
  lidt (IDT_DESCRIPTOR)

  mov %cr0, %eax
  or $1, %eax
  mov %eax, %cr0

  ljmp $0x8, $protected

.code32
protected:
  # setup registers with appropriate GDT values
  mov $0x10, %eax
  mov %eax, %ds
  mov %eax, %es
  mov %eax, %fs
  mov %eax, %gs
  mov %eax, %ss

  # jump to second stage code
  ljmp $0x08, $_ss_start

.code16

  # ############################################
  # print
  #   prints out a string on the screen
  #   %si: points to the message to be printed
  # ############################################
print:
  movw $0x0000, %bx
  movb $0x0E, %ah

print_loop:
  lodsb
  or %al, %al
  jz print_end
  int $0x10
  jmp print_loop
print_end:
  ret


  # some data
message: 
  .ascii "Hello World!"
  .byte 13
  .byte 10
  .byte 0

GDT_DESCRIPTOR:
  .word 0x23
  .int GDT

IDT_DESCRIPTOR:
  .word 0
  .int 0

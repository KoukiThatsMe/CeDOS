.section .text

  # disable interrupts
  cli

  # canonicalize %CS:%EIP to a known value
  ljmp $0, $start
start:

  # setup segments
  mov $0x0000, %ax
  mov %ax, %ss
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs

  # setup stack
  mov $0x9000, %ax
  mov %ax, %bp
  mov %ax, %sp

  # select display page 0
  mov $0, %al
  mov $0x05, %ah
  int $0x10

  # print hello world
  mov $message, %si
  mov $0x0000, %bx
  mov $0x0E, %ah

print_loop:
  lodsb
  or %al, %al
  jz print_end
  int $0x10
  jmp print_loop
print_end:

  # TODO:
  # - load kernel into memory starting at adress 0x10000
  #   (NOTE: this code can only be jumped to after loading the global descriptor table
  #          because it uses absolute adresses larger than 16 bit)
  # - load global descriptor table and far jump into protected mode
  # - activate A20 gate
  # - jump to low kernel code

  # loop forever

loop:
  jmp loop

  # some data
message: 
  .ascii " Hello World!"
  .byte 0x00

end:
  .=510
  .byte 0x55
  .byte 0xAA

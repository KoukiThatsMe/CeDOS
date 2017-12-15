.section .text
start:
# loop:
#   jmp loop

low_kernel_welcome:
  .ascii "Welcome to the low kernel!"
  .byte 13
  .byte 10
  .byte 0

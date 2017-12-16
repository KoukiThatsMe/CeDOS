.section .text
.global _start
_start:
loop:
  jmp loop

.section .data
low_kernel_welcome:
  .ascii "Welcome to the low kernel!"
  .byte 13
  .byte 10
  .byte 0

.section .text
.global _kernel_start
_kernel_start:
  call main

loop:
  jmp loop

.section .data
kernel_msg:
  .ascii "Welcome to the high kernel. Please take a seat and enjoy the fucking show"
  .byte 0
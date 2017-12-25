.section .text
.global _kernel_start
_kernel_start:
  # move stack to kernel space
  mov $__KERNEL_STACK_ADDR, %eax
  mov %eax, %esp
  mov %esp, %ebp

  # call main
  call main

loop:
  jmp loop

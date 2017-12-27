.section .text
.global _kernel_start
_kernel_start:
  # move stack to kernel space
  mov $__KERNEL_STACK_ADDR, %eax
  mov %eax, %esp
  mov %esp, %ebp

  # initialize kernel
  call os_init
  jz error

  # call main
  call os_main

loop:
  jmp loop

error:
  jmp error
.section .text
.global __KERNEL_START
__KERNEL_START:
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

.section .text
.global _ss_start
_ss_start:

  # this code prints out all ascii characters in reverse order in
  # rainbow colors just to test if this point of the code was reached
  # (you might call it overkill, i call it fabulous)
  mov $255, %ecx
  mov $0xB8000, %esi

test:
  movb %cl, (%esi)
  inc %esi
  movb %cl, (%esi)
  inc %esi
  loop test


  # TODO:
  #  - copy the kernel code to 0x00100000
  #  - create a page directory for the kernel that maps it to 0xC0000000
  #    and identity maps the first 1M of memory
  #  - enable paging
  #  - jump to kernel code


  # loop until the heat death of the universe
loop:
  jmp loop

.section .data
low_kernel_welcome:
  .ascii "Welcome to the low kernel!"
  .byte 13
  .byte 10
  .byte 0

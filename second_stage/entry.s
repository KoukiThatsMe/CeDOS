.section .text
.global _ss_start
_ss_start:
  # copy the kernel code to 0x00100000
  call copy_kernel

  # create a page directory for the kernel that maps it to 0xC0000000
  # and identity maps the first 1M of memory
  call create_kernel_environment
  
  # load kernel page directory
  movl %eax, %cr3

  # set paging enable and protection bit
  movl %cr0, %eax
  or $0x80000010, %eax
  movl %eax, %cr0

  # jump to kernel code
  ljmp $8, $__KERNEL_VMA

  # loop until the heat death of the universe
loop:
  jmp loop
  

.section .data
low_kernel_welcome:
  .ascii "Welcome to the low kernel!"
  .byte 13
  .byte 10
  .byte 0

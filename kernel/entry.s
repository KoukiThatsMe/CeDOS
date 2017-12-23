.global _kernel_start
_kernel_start:
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

  # loop until the heat death of the universe
loop:
  jmp loop

  .ascii "hey now youre an allstar get your game on"

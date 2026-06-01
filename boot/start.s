.section .text.boot
.global _start

_start:
  # Inicializa gp
  la gp, __global_pointer$

  # Inicializa stack
  la sp, kernel_stack_top

  # Limpa BSS
  la t0, __bss_start
  la t1, __bss_end

1:
  bgeu t0, t1, 2f
  sd zero, 0(t0)
  addi t0, t0, 8
  j 1b

2:
  call kernel_main

3:
  j 3b

.section .bss
.align 12

kernel_stack:
  .space 4096

kernel_stack_top:

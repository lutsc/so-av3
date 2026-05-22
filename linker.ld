ENTRY(_start)

SECTIONS
{
  . = 0x80200000;
  
  .text : {
    *(.text.boot)
    *(.text*)
  }

  .rodata : {
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .bss : ALIGN(4096) {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  }

  __global_pointer$ = . + 0x800;
}

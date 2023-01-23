OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

ENTRY (_start)

SECTIONS
{
  . = 0x00100000;

  .text :
  {
    *(.multiboot)
    *(.text)
    *(.rodata)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.COMMON)
    *(.bss)
    *(.stack)
  }

  /DISCARD/ :
  {
    *(.eh_frane)
  }
}

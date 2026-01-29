PHDRS
{
  text PT_LOAD FILEHDR PHDRS ;
}

SECTIONS
{
  . = 0x08048000 + SIZEOF_HEADERS;

  .text : {
    *(.text*)
    *(.rodata*)
    *(.data*)
    *(.bss*)
  } :text

  /DISCARD/ : {
    *(.comment)
    *(.note*)
    *(.eh_frame*)
    *(.gnu*)
  }
}

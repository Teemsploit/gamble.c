PHDRS
{
  text PT_LOAD FILEHDR PHDRS ;
}

SECTIONS
{
  . = 0x400000 + SIZEOF_HEADERS;

  .text : {
    *(.text*)
    *(.rodata*)
    *(.data*)
  } :text

  /DISCARD/ : {
    *(.comment)
    *(.note*)
    *(.eh_frame*)
    *(.gnu*)
    *(.hash)
  }
}

MEMORY
{
	Vectors : ORIGIN = 0x80000000, LENGTH = 0x00002100	
	ROM     : ORIGIN = 0x80002100, LENGTH = 0x03FFDF00
	SRAM    : ORIGIN = 0x00000004, LENGTH = 0x03FEFFFC
}

ENTRY(initStack);

SECTIONS
{  
	.vectors :
	{ 
	} >Vectors
	
	.text : 
	{  	
		__TEXT_START__ = .;
		*(.text)
		*(.startup)
		*(.gnu.warning)
		*(.gnu.linkonce.t*)
		*(.gnu.linkonce.s*)
		*(.init)
		*(.fini)
		*(.gnu.linkonce.r*)
		*(.rodata1)
		*(.gcc_except_table)    
		*(.jcr) 
		*(.rodata.str1.4)
		*(.rodata)
		*(.eh_frame)
		PROVIDE (__CTOR_LIST__ = .);
		*(.ctors)
		PROVIDE (__CTOR_END__ = .);
		PROVIDE (__DTOR_LIST__ = .);
		*(.dtors)
		PROVIDE (__DTOR_END__ = .);
		PROVIDE (_FIXUP_START_ = .);
		*(.fixup)
		PROVIDE (_FIXUP_END_ = .);
		__TEXT_END__ = .;  
	} >ROM =0
  
	.data : AT( ADDR(.text) + SIZEOF(.text) )
	{
		__DATA_START__ = .;  
		*(.data)
		*(.gnu.linkonce.d*)    
		*(.data1)
		*(.got1)
		*(.data.rel.local) 
		*(.data.rel)
		*(.data.rel.ro.local)
		PROVIDE (_GOT2_START_ = .);
		*(.got2)
		PROVIDE (_GOT2_END_ = .);
		PROVIDE (_GOT_START_ = .);
		*(.got)
		*(.got.plt)
		PROVIDE (_GOT_END_ = .);
		__SDATA_START__ = .; 
		_SDA_BASE_ = .; 
		*(.sdata)
		__SDATA_END__ = .; 
		__SDATA2_START__ = .; 
		_SDA2_BASE_ = .;
		*(.sdata2)
		__SBSS2_START__ = .; 
		*(.sbss2)
		__SBSS2_END__ = .; 
		__SDATA2_END__ = .; 
		__DATA_END__ = .;
	} >SRAM
  
	.bss :
	{
		__BSS_START__ = .;  
		PROVIDE (__sbss_start = .);
		__SBSS_START__ = .; 
		*(.sbss)
		__SBSS_END__ = .; 
		*(.scommon)
		PROVIDE (__sbss_end = .); 
		PROVIDE (__bss_start = .);
		*(.dynbss)
		*(.bss)
		*(COMMON)
		__BSS_END__ = .;  
	} >SRAM
  
	.stack :
	{ 
		. += 0x10000;
		stackPointer = .;
	} >SRAM
  
	/* DWARF 1 */
	.debug           0 : { *(.debug) }
	.line            0 : { *(.line) }
	.debug_srcinfo   0 : { *(.debug_srcinfo) }
	.debug_sfnames   0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges   0 : { *(.debug_aranges) } 
	.debug_pubnames  0 : { *(.debug_pubnames) } 
	/* DWARF 2 */
	.debug_info      0 : { *(.debug_info) } 
	.debug_abbrev    0 : { *(.debug_abbrev) } 
	.debug_line      0 : { *(.debug_line) } 
	.debug_frame     0 : { *(.debug_frame) } 
	.debug_str       0 : { *(.debug_str) } 
	.debug_loc       0 : { *(.debug_loc) } 
	.debug_macinfo   0 : { *(.debug_macinfo) } 
	.debug_ranges    0 : { *(.debug_ranges) } 
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) } 
	.debug_funcnames 0 : { *(.debug_funcnames) } 
	.debug_typenames 0 : { *(.debug_typenames) } 
	.debug_varnames  0 : { *(.debug_varnames) }
}

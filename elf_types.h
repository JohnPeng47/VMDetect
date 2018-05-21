#define EI_NIDENT 16

struct Elf32_Ehdr
{
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  uint16_t	e_type;			/* Object file type */
  uint16_t	e_machine;		/* Architecture */
  uint32_t	e_version;		/* Object file version */
  uint32_t 	e_entry;		/* Entry point virtual address */
  uint32_t	e_phoff;		/* Program header table file offset */
  uint32_t	e_shoff;		/* Section header table file offset */
  uint32_t	e_flags;		/* Processor-specific flags */
  uint16_t	e_ehsize;		/* ELF header size in bytes */
  uint16_t	e_phentsize;		/* Program header table entry size */
  uint16_t	e_phnum;		/* Program header table entry count */
  uint16_t	e_shentsize;		/* Section header table entry size */
  uint16_t	e_shnum;		/* Section header table entry count */
  uint16_t	e_shstrndx;		/* Section header string table index */
}; 

struct Elf32_Sym
{
  uint32_t	st_name;		/* Symbol name (string tbl index) */
  uint32_t	st_value;		/* Symbol value */
  uint32_t	st_size;		/* Symbol size */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char	st_other;		/* Symbol visibility */
  uint16_t	st_shndx;		/* Section index */
};




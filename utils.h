#include <stdint.h>

Elf32_Shdr* get_section(Elf32_Shdr* shdr, int sh_index);
Elf32_Shdr* get_section_ptr(Elf32_Ehdr* ehdr, char* file);

uint32_t get_sym_addr(char* file, Elf32_Sym* sym, Elf32_Ehdr* ehdr, Elf32_Shdr* strtab);
uint32_t get_sym_value(char* filename, char* sym_name);

uint32_t resolve_va(char* file, uint32_t va);
Elf32_Phdr* get_phdrs_ptr(Elf32_Ehdr* ehdr, char* file);


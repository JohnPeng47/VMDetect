#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <elf.h>
#include <fcntl.h>
#include "utils.h"
	
#ifdef DEBUG
#define PRINTDEBUG(x) printf x //variable number of arguments
#else
#define PRINTDEBUG(x) do{} while(0)
#endif


uint32_t get_sym_value(char* file, char* sym_name){ 
	Elf32_Ehdr* ehdr;
	Elf32_Shdr* shdr; //generic entry for enumerating sections
	Elf32_Shdr strtab; //holds string in symtab
	Elf32_Shdr symtab;
	char* sh_strtab; //hold sections names
	Elf32_Sym* sym;

	char* mem = file;

	ehdr = (Elf32_Ehdr *)mem;
	shdr = (Elf32_Shdr* )(mem + ehdr->e_shoff);
	PRINTDEBUG(("number of section headers: %d\n", ehdr->e_shnum)); //need double brackets for variable #of arguments

	
	sh_strtab = (char *)(mem + (shdr[ehdr->e_shstrndx].sh_offset)); 

	//find address of symtab and strtab	
	for(int i = 0; i < ehdr->e_shnum; i++){
		if(shdr[i].sh_size){

			printf("%s\n", &sh_strtab[shdr[i].sh_name]);
	
			if(strcmp(&sh_strtab[shdr[i].sh_name], ".strtab") == 0)
				strtab = shdr[i];
			if(strcmp(&sh_strtab[shdr[i].sh_name], ".symtab") == 0)
				symtab = shdr[i];			

		}
	}

	PRINTDEBUG(("symtab offset %x\n", symtab.sh_offset));
	PRINTDEBUG(("strtab offset %x\n", strtab.sh_offset));
	
	char* symtab_str = (char *)(mem + strtab.sh_offset);
	sym = (Elf32_Sym* )(mem + symtab.sh_offset);
	
	
	printf("Symbol names: \n");
	for(int i = 0;  i < (symtab.sh_size / symtab.sh_entsize); i++, sym++){
		printf("name: %s, st_name: %d\n",&symtab_str[sym->st_name], sym->st_name);
		if(strcmp(&symtab_str[sym->st_name], sym_name) ==0)	
					
			//printf("name: %s, st_name: %d\n",&symtab_str[sym->st_name], sym->st_name);
			//TODO: resolve reloc'd syms
			return get_sym_addr(mem, sym, ehdr, &strtab);
	}

}

Elf32_Shdr* get_section_ptr(Elf32_Ehdr* ehdr, char* file){
	return (Elf32_Shdr* )(file + ehdr->e_shoff);
}

Elf32_Shdr* get_section(Elf32_Shdr* shdr, int sh_index){
	return &shdr[sh_index];
}

uint32_t get_sym_addr(char* file, Elf32_Sym* sym, Elf32_Ehdr* ehdr, Elf32_Shdr* strtab){ //strtab used for relocations
	if(sym->st_shndx == SHN_UNDEF){ //TODO
		return 0;
	}
	else if(sym->st_shndx == SHN_ABS){
		printf("abs\n");
		return sym->st_value;
	}
	else{
		printf("rel\n");
		Elf32_Shdr* shdr = get_section_ptr(ehdr, file);
		Elf32_Shdr* sym_section = get_section(shdr, sym->st_shndx);
		printf("section offset: %x\n", sym_section->sh_offset);
		printf("file start: %x\n", file);
		printf("sym value: %x\n", sym->st_value);
		return file + sym_section->sh_offset + sym->st_value;
	}
}
	

		 

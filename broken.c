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


uint32_t main(int argc, char** argv){
	char* filename = argv[1];
	char* sym_name = argv[2];
	int fd = open(filename, O_RDONLY);
	struct stat st;
	stat(fd, &st);

	char mem[st.st_size];
	read(fd, mem, st.st_size);
 
	Elf32_Ehdr* ehdr;
	Elf32_Shdr* shdr; //generic entry for enumerating sections
	Elf32_Shdr strtab; //holds string in symtab
	Elf32_Shdr symtab;
	char* sh_strtab; //hold sections names
	Elf32_Sym* sym;


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
	    printf("%x\n",&symtab_str[sym->st_name]);
	    if(strcmp(&symtab_str[sym->st_name], sym_name) ==0) 
			printf("not crahsed\n");

		//TODO: resolve reloc'd syms
	}
}

		 

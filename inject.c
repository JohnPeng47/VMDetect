#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <elf.h>
#include <fcntl.h>

void main(int argc, char** argv){
	const char* filename = argv[1];
	int fd;
	Elf32_Ehdr* ehdr;
	Elf32_Shdr* shdr; 
	Elf32_Sym* sym;
	struct stat st;

	fd = open(filename, O_RDONLY);	
	stat(filename, &st);
	char mem[st.st_size];

	read(fd, mem, st.st_size);
	
	ehdr = (Elf32_Ehdr *)mem;
	printf("Identifier: %s\n", ehdr->e_ident);
	
	shdr = (Elf32_Shdr* )(mem + ehdr->e_shoff);
	printf("number of section headers: %d\n", ehdr->e_shnum);
	
	for(int i = 0;  i < ehdr->e_shnum; shdr++, i++){
		//printf("start: %p, type: %d\n", shdr, shdr->sh_type);
		if(shdr->sh_type == SHT_SYMTAB){
			printf("size of sym tab: %d\n",(shdr->sh_size / shdr->sh_entsize));
			//read section into memory
			


			sym = (Elf32_Sym* )(mem + shdr->sh_offset);
			//symbol parsing code in get_32bit_elf_symbols, readelf
			for(int sym_index = 0;  sym_index < (shdr->sh_size / shdr->sh_entsize) - 5; sym++, sym_index++){
				printf("name: %s, addr: &p\n", sym->st_name, sym->st_value);			
			}
		}
	}
}

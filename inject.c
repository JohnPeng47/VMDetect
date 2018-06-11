#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <elf.h>
#include <fcntl.h>
	
#include "utils.h"
#define PAGE_SIZE 4096
#ifdef DEBUG
#define PRINTDEBUG(x) printf x //variable number of arguments
#else
#define PRINTDEBUG(x) do{} while(0)
#endif

void main(int argc, char** argv){

	if(argc < 2){
		printf("Usage: ./inject filename sym_name\n");
		exit(0); 
	}
	Elf32_Ehdr* ehdr;
	Elf32_Phdr* phdr;
	Elf32_Shdr* shdr;

	const char* filename = argv[1];
	const char* sym_name = argv[2];
	const char* jmp_name = argv[3]; //name of function to jump to; replace later replace with code to be executed

	uint32_t inject_site; 
	int fd;
	struct stat st;

	fd = open(filename, O_RDONLY);	
	stat(filename, &st);
	char mem[st.st_size];

	read(fd, mem, st.st_size);
	PRINTDEBUG(("starting address: %x\n", mem));
	//parse symbol table to determine the virtual address of function
	uint32_t sym_addr_va= get_sym_value(mem, sym_name);	
	PRINTDEBUG(("target function va @ %p\n", sym_addr_va));
	
	//convert va to offset from physical file
	uint32_t sym_file_offset = resolve_va(mem, sym_addr_va);
	PRINTDEBUG(("file offset @ %d bytes\n", sym_file_offset));

	//get va of address to jump to
	uint32_t jmp_addr_va = get_sym_value(mem, jmp_name);
	PRINTDEBUG(("target jump addr @ %x\n", jmp_addr_va));

	//function to modify the 

	ehdr = (Elf32_Ehdr* )mem;
	phdr = (Elf32_Phdr* )(mem + ehdr->e_phoff); //need brackets around whole expression for typecasting
	shdr = (Elf32_Shdr* )(mem + ehdr->e_shoff);

	int jmp_code_sz = 6;
	char jmp_code[6];

	jmp_code[0] = '\x68';
	jmp_code[1] = '\x00';
	jmp_code[2] = '\x00';
	jmp_code[3] = '\x00';
	jmp_code[4] = '\x00';
	jmp_code[5] = '\xc3';
	// jmp_code[6] = 0;  this caused opcodes to be misaligned, and resulted in faulty disassembly

	
	*(uint32_t *)&jmp_code[1] = jmp_addr_va; //patch with the destination address
	printf("%x\n", jmp_code[1]);
 	//coorect the program fields
	int found = 0;
	unsigned int new_end_of_text;
	unsigned int jmp_addr;  
	unsigned int segment_size; //size of program header segment
	int out;

	for(int i = ehdr->e_phnum; i-- > 0; phdr++){
		//printf("fileoffset: %d, memsz: %d, filesize: %d, type: %d\n", phdr->p_offset, phdr->p_memsz, phdr->p_filesz, phdr->p_type);
		if(found){
			PRINTDEBUG(("Increasing p_offset\n"));
			phdr->p_offset += PAGE_SIZE;
		}
		else if(sym_file_offset >= phdr->p_offset && sym_file_offset < phdr->p_offset + phdr->p_filesz){
			PRINTDEBUG(("modifying offsets\n"));
			new_end_of_text = phdr->p_offset + phdr->p_filesz;
			jmp_addr = phdr->p_vaddr + phdr->p_filesz;
			segment_size = phdr->p_filesz;
			phdr->p_filesz += jmp_code_sz;
			phdr->p_memsz += jmp_code_sz;
			PRINTDEBUG(("Found segment @ %x, end of text: %x, new filesize: %x, memsz: %x\n",  phdr->p_vaddr, new_end_of_text, phdr->p_filesz, phdr->p_memsz));
			found++;
		}
	}	

	found = 0;
	for(int i = ehdr->e_shnum; i-- > 0; shdr++){	
		if (shdr->sh_offset >= new_end_of_text)
			shdr->sh_offset += PAGE_SIZE;
	  	else if (shdr->sh_size + shdr->sh_addr == jmp_addr) //if the end of the section lands on the jump code, then
		  													//increase size of the section to accomodate for it
		  	shdr->sh_size += jmp_code_sz;
		// unsigned int old_offset = shdr->sh_offset;
		// if(found){
		// 	shdr->sh_offset += PAGE_SIZE;	
		// }
		// else if (sym_addr_va >= shdr->sh_addr && sym_addr_va < (shdr->sh_addr + shdr->sh_size)){
		// 	PRINTDEBUG(("Found section, #%d\n", ehdr->e_shnum - i - 1));
		// 	shdr->sh_size += jmp_code_sz;
		// 	found++;
		// }
		//PRINTDEBUG(("Section #%d new offset @ %x, old offset: %x, vaddr: %x, link: %d\n", ehdr->e_shnum - i - 1, shdr->sh_offset, old_offset, shdr->sh_addr, shdr->sh_link));	
	}
	ehdr->e_shoff += PAGE_SIZE;
	
	//write our changes to file           
	out = open("./tmp", O_CREAT | O_WRONLY | O_TRUNC, st.st_mode);
	write(out, mem, sym_file_offset); //write up to entry of function we wish to redirect
	write(out, jmp_code, jmp_code_sz);

	//write a page of padding
	write(out, mem + (int)sym_file_offset, segment_size); //write the rest of program segment 
	lseek(out, PAGE_SIZE - jmp_code_sz, SEEK_CUR); //lseek 
	
	write(out, mem + (int)(sym_file_offset + segment_size), st.st_size - sym_file_offset);
	close(out);
}
	
	


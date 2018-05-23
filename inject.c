#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <elf.h>
#include <fcntl.h>
	
#include "utils.h"

void main(int argc, char** argv){

	if(argc < 2){
		printf("Usage: ./inject filename sym_name\n");
		exit(0);
	}

	const char* filename = argv[1];
	const char* sym_name = argv[2];
	int fd;
		
	struct stat st;

	fd = open(filename, O_RDONLY);	
	stat(filename, &st);
	char mem[st.st_size];

	read(fd, mem, st.st_size);
	//parse symbol table to determine the virtual address of function
	uint32_t sym_addr_va= get_sym_value(mem, sym_name);	
	printf("starting address: %x\n", mem);
	printf("sym virtual addr @ %p\n", sym_addr_va);
	
	//at this point, we need to search program headers for 
	uint32_t sym_addr_file = resolve_va(mem, sym_addr_va);
	
	
}
	
	


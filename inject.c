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
	uint32_t sym_addr = get_sym_value(mem, sym_name);	
	printf("starting address: %x\n", mem);
	printf("sym addr @ %p\n", sym_addr);
	
}
	
	


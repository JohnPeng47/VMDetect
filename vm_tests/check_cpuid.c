#include <stdio.h>
#include <cpuid.h>

void main(){
	int input=1, output;
	__asm__ __volatile__( 
		  "mov %1, %%eax;"
		  "cpuid;"
		  "mov %%ecx, %0;"
		  :"=r"(output) 
		  :"r"(input) 
		  :"%eax", "%ebx", "%ecx", "%edx"
	);
	printf("ecx: %d, %h\n", output, output);
}

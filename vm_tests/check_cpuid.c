#include <stdio.h>
#include <cpuid.h>
#include <stdint.h>

void main(){
	uint32_t EBX;
	uint32_t ECX;
	uint32_t EDX;
	
	/*
	Some inline assembly notes:
	- syntax is at&t, so in the form: op src, dst
	- asm args must be declared in order: assembly, outputs, inputs, clobber
	- if n output and input args, then first output arg is %0, and last input arg is %n-1
	*/

	__asm__( 
		  "mov $0x40000000, %%eax\n" 
		  "cpuid\n"
		  "mov %%ebx, %0\n"
		  "mov %%ecx, %1\n"
		  "mov %%edx, %2\n"	
		  :"=m"(EBX),"=m"(ECX),"=m"(EDX) //r modifier means EBX stores a register value
		  :
		  :"%eax", "%ebx", "%ecx", "%edx"
	);
	printf("ebx: %x\n", EBX);
}

; Define variables in the data section
SECTION .DATA
	set:     db 'VM bit is set', 10 ;10 is the decimal ascii value of '\n'
	setLen:  equ $-set
	notset:	 db 'VM bit is not set', 10
	notsetLen: equ $-notset

; Code goes in the text section
SECTION .TEXT
	GLOBAL main

main:
	mov eax, 1
	cpuid
	bt ecx, 0x1f		;bt copies the bit at [operand] to the carry bit in EFLAGS
	and cx, 0x80		
	jc vm				;branches if the carry bit is set

not_vm:
	mov eax,4            ; 'write' system call = 4
	mov ebx,1            ; file descriptor 1 = STDOUT
	mov ecx,notset        ; string to write
	mov edx,notsetLen     ; length of string to write
	int 80h              ; call the kernel
	jmp exit

vm:
	mov eax,4            ; 'write' system call = 4
	mov ebx,1            ; file descriptor 1 = STDOUT
	mov ecx,set        ; string to write
	mov edx,setLen     ; length of string to write
	int 80h              ; call the kernel
	

exit:
	; Terminate program
	mov eax,1            ; 'exit' system call
	mov ebx,0            ; exit with error code 0
	int 80h              ; call the kernel

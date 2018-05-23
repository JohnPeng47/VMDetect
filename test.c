#include <stdio.h>

void func2(char* printme);

void main(){
	const char* hello = "hello world";
	func1(hello);
}

void func1(char* printme){
	printf("%s\n", printme);
}


#include <stdio.h>

void main(){
	const char* hello = "hello world";
	func1(hello);
}

void func1(char* printme){
	printf("%s\n", printme);
}

void func2(){
	printf("func2 called\n");
}

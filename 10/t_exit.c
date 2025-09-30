#include <func.h>

void func1(void){
	printf("Func1");
}

void func2(void){
	printf("Func2");
}

void func(int status, void* arg){
	printf("arg= %ld ", (long) arg);
}

int main(void){
	//退出处理函数
	atexit(func1);
	atexit(func2);
	atexit(func2);
	on_exit(func,(void*) 10);

	printf("Program is running...\n");

	printf("Program is going to die...");
	exit(1);

	return 0;
}

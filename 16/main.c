#include <func.h>

int main()
{
    int a = 0x41424344;
	char *c = (char*) &a;
	printf("First character: %c\n",*c);
	//输出的字符为D，ASCII码对应十六进制是0x44，低地址存储的是数据的低字节部分，说明机器采用小端模式
	printf("second character: %c\n", *(c+1));
    return 0;
}


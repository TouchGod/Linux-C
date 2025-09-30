#include <func.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		error(1,0,"Usage: %s test", argv[0]);
	}

	int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd == -1){
		error(1, errno, "open");
	}

	//截断
	if(ftruncate(fd, 40959) == -1){
		error(1,errno,"ftruncate");
	}

	//文件位置偏移
	if(lseek(fd,0,SEEK_END) == -1){
		error(1,errno,"lseek");
	}

	//写入
	char buf[] = "Hello world";
	int len = strlen(buf);
	if(write(fd, buf, len) == -1){
		error(1,errno,"write");
	}

	//fstat
	struct stat sb;
	if(fstat(fd,&sb) == -1){
		error(1, errno, "fstat");
	}
	printf("st_blocks: %ld\n", sb.st_blocks);

	close(fd);
}

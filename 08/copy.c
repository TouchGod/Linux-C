#include <func.h>

int main(int argc, char *argv[])
{
   // ./copy src dst
	if(argc != 3){
		error(1, 0, "Usage: %s src dst",argv[0]);
	}

	int src = open(argv[1], O_RDONLY);
	if(src == -1){
		error(1, errno, "open %s failed!", argv[1]);
	}

	int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(dst == -1){
		error(1, errno, "open %s failed!", argv[2]);
	}

	int n;
	char buf[4096];
	while((n = read(src, buf, sizeof(buf))) > 0){
		write(dst, buf, n);
	}
	if (n == -1){
		error(1, errno, "read");
	}

	close(src);
	close(dst);
	return 0;
}

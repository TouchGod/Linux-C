#include <func.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		error(1, 0, "Usage: %s src dst", argv[0]);
	}

	int src = open(argv[1], O_RDONLY);
	if(src == -1){
		error(1, errno, "open %s failed",argv[1]);
	}
	int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(dst == -1){
		error(1, errno, "open %s failed",argv[1]);
	}

	int fsize = lseek(src, 0, SEEK_END);
	ftruncate(dst, fsize);
	lseek(src, 0, SEEK_SET);

	char buf[4096];
	char empty[4096]={'\0'};

	ssize_t rn, wn;

	while((rn = read(src, buf, 4096)) > 0){
		if(memcmp(buf, empty, rn) == 0){
			lseek(dst, rn, SEEK_CUR);
		}else{
			wn = write(dst, buf, rn);
		}

		if(wn == -1){
			error(1, errno, "write");
		}
	}

	close(src);
	close(dst);

	if(rn == -1){
		error(1, errno, "read");
	}

	return 0;
}

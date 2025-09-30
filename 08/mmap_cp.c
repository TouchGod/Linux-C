#include <func.h>

#define MMAP_SIZE 4096

int main(int argc, char* argv[])
{
    // ./mmap_cp src dst
	if(argc != 3){
		error(1,0,"Usage: %s src dst", argv[0]);
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		error(1,errno,"open %s failed.", argv[1]);
	}

	int fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd2 == -1){
		error(1, errno, "open %s failed.", argv[2]);
	}

	struct stat sb;
	if(fstat(fd1, &sb) == -1){
		error(1, errno, "fstat");
	}
	int fsize = sb.st_size;
	if(ftruncate(fd2, fsize) == -1){
		error(1, errno, "ftruncate");
	}

	int offset = 0;
	while(offset < fsize){
		int length;//映射区大小
		if(fsize - offset >= MMAP_SIZE){
			length = MMAP_SIZE;
		}else{
			length = fsize - offset;
		}

		//映射
		void* add1 = mmap(NULL, length, PROT_READ, MAP_SHARED, fd1, offset);
		if(add1 == MAP_FAILED){
			error(1,errno,"mmap %s", argv[1]);
		}
		void* add2 = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, offset);
		if(add2 == MAP_FAILED){
			error(1, errno, "mmap %s", argv[2]);
		}

		//操作内存
		memcpy(add2, add1, length);

		munmap(add1,length);
		munmap(add2,length);

		offset += length;
	}

	close(fd1);
	close(fd2);
	return 0;
}

#include <func.h>

#define MAXSIZE 256

int main(int argc, char* argv[]){
	//1.pipe()
	int fields1[2];
	int fields2[2];

	if(pipe(fields1) == -1){
		error(1, errno, "pipe");
	}

	if(pipe(fields2) == -1){
		error(1, errno, "pipe");
	}

	//2.fork()
	char message[MAXSIZE];
	pid_t pid =fork();
	switch (pid){
	case -1:
		error(1, errno, "fork");
	case 0:
		//3.子进程关闭一端
		close(fields1[1]);
		close(fields2[0]);
		write(fields2[1], "I am your child.\n", 17);
		read(fields1[0], message, MAXSIZE);
		printf("Child received: %s", message);
		exit(1);
	default:
		//4.父进程关闭另一端
		close(fields1[0]);
		close(fields2[1]);
		read(fields2[0], message, MAXSIZE);
		printf("Parent received: %s", message);
		write(fields1[1], "I am your father.\n",18);
		exit(1);
	}

	return 0;
}

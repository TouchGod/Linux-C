#include <func.h>

#define MAXLINE 256

int main(int argc, char* argv[]){
	int fd1 = open("pipe1", O_RDONLY);
	if(fd1 == -1){
		error(1, errno, "open");
	}

	int fd2 = open("pipe2", O_WRONLY);
	if(fd2 == -1){
		error(1, errno, "open");
	}

	printf("Established!\n");

	char recvline[MAXLINE];
	char sendline[MAXLINE];

	fd_set mainfds;
	FD_ZERO(&mainfds);
	FD_SET(STDIN_FILENO, &mainfds);
	int maxfd=STDIN_FILENO;//	nfds
	FD_SET(fd1, &mainfds);
	if(fd1 > maxfd){
		maxfd = fd1;
	}

	for(;;){
		fd_set readfds = mainfds;
		int events = select(maxfd + 1, &readfds, NULL, NULL, NULL);
		switch(events){
		case -1:
			error(1, errno, "select");
		case 0:
			//timeout
			printf("Time out\n");
			break;
		default:
			if(FD_ISSET(STDIN_FILENO, &readfds)){
				if(fgets(sendline, MAXLINE, stdin) != NULL){
					write(fd2, sendline, strlen(sendline) + 1);
				}
			}
			if(FD_ISSET(fd1, &readfds)){
				int bytes = read(fd1, recvline, MAXLINE);
				if(bytes == 0){
					goto end;
				}
				else if(bytes == -1){
					error(1, errno, "read");
				}
				printf("from p2: %s",recvline);
			}
		}
	}
end:	close(fd1);
		close(fd2);

	return 0;
}

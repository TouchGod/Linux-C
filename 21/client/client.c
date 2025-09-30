#include <header.h>

int recvn(int netfd, void *buf, int length){
	int n = 0;
	while(n < length){
		int ret = recv(netfd, buf + n, length - n, 0);
		if(ret == 0){
			break;
		}
		n += ret;
	}
	printf("read %d\n",n);
	return n;
}

int main(){

	char *ip = "192.168.161.128";
	char *port = "8080";

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(atoi(port));

	connect(socketfd, (struct sockaddr *)&addr, sizeof(addr));

	//读取文件名
	int name_len = 0;
	recv(socketfd, &name_len, sizeof(name_len), 0);
	char file_name[60] = {0};
	recv(socketfd, file_name, name_len, 0);

	int file_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);

	// 读取文件内容
	while(1){
		int buf_len=0;
		int ret;
		/* ret = recv(socketfd, &buf_len, sizeof(int), MSG_WAITALL); */
		printf("-----------1-----------\n");
		ret = recvn(socketfd, &buf_len, sizeof(int));
		printf("-----------2-----------\n");
		if(ret == 0){
			break;
		}

		char buf[1000]={0};
		/* ret = recv(socketfd, buf, buf_len, MSG_WAITALL); */
		ret = recvn(socketfd, buf, buf_len);
		if(ret == 0){
			break;
		}
		write(file_fd, buf, buf_len);
		printf("******************\n");
	}
	close(file_fd);
	close(socketfd);

	return 0;
}

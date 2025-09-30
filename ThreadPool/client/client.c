#include <header.h>

int main(){

	char *ip = "192.168.161.128";
	char *port = "8080";

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(atoi(port));

	connect(socketfd, (struct sockaddr *)&addr, sizeof(addr));

	// 读取文件名
	int file_name_len=0;
	recv(socketfd, &file_name_len, sizeof(int), 0);
	char file_name[60]={0};
	recv(socketfd, file_name, file_name_len, 0);

	// 接受文件大小
	off_t file_size=0;
	recv(socketfd, &file_size, sizeof(off_t), MSG_WAITALL);

	// 接收文件内容
	int file_fd = open(file_name, O_RDWR|O_CREAT, 0600);

	ftruncate(file_fd, file_size);// 先调整文件大小

	void *p = mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, file_fd, 0 );

	recv(socketfd, p, file_size, MSG_WAITALL );

	munmap(p, file_size);

	close(file_fd);

	close(socketfd);

	return 0;
}

#include <header.h>

int main(void){
	char* port = "8080";
	char* ip = "192.168.161.128";

	//socket
	int socket_fd  = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	//bind
	int res_bind = bind(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	ERROR_CHECK(res_bind, -1, "bind");

	//listen
	int res_listen = listen(socket_fd, 10);
	ERROR_CHECK(res_listen, -1, "listen");

	int new_fd = accept(socket_fd, NULL, NULL);
	ERROR_CHECK(new_fd, -1, "accept");

	int name_len;
	recv(new_fd, &name_len, sizeof(name_len), 0);
	char name[60] = {0};
	recv(new_fd, name, name_len, 0);

	int file_size;
	recv(new_fd, &file_size, sizeof(file_size), 0);
	char buf[100] = {0};
	recv(new_fd, buf, file_size, 0);

	int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ERROR_CHECK(fd, -1, "open");

	write(fd, buf, file_size);

	close(fd);
	close(new_fd);
	close(socket_fd);

	return 0;
}

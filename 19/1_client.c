#include <header.h>

int main(void){
	char* ip = "192.168.161.128";
	char* port = "8080";

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	connect(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	while(1){
		char file_name[20] = {0};
		scanf("%s", file_name);
		int name_len = strlen(file_name);
		send(socket_fd, &name_len, sizeof(int), 0);
		send(socket_fd, file_name, name_len, 0);

		int file_len;
		char file[100] = {0};
		recv(socket_fd, &file_len, sizeof(int), 0);
		recv(socket_fd, file, file_len, 0);

		int file_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		write(file_fd, file, file_len);
		close(file_fd);
	}
	close(socket_fd);
	return 0;
}

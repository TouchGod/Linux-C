#include <header.h>

int main(void){
	char* ip = "192.168.161.128";
	char* port = "8080";

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	listen(socket_fd, 5);

	int new_fd = accept(socket_fd, NULL, NULL);

	while(1){
		int name_len;
		char file_name[20] = {0};
		int res_recv = recv(new_fd, &name_len, sizeof(int), 0);
		if(res_recv == 0){
			break;
		}
		recv(new_fd, file_name, name_len, 0);

		char file[100] = {0};
		int file_fd = open(file_name, O_RDONLY);
		int file_len = read(file_fd, file, sizeof(file));
		send(new_fd, &file_len, sizeof(int), 0);
		send(new_fd, file, file_len, 0);
		close(file_fd);
	}
	close(new_fd);
	close(socket_fd);
	return 0;
}

#include "head.h"

int initTcpSocket(int *socket_fd, char *ip, char *port){
	*socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	int reuse = 1;
	setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	bind(*socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	listen(*socket_fd, 10);

	return 0;
}

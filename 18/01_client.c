#include <header.h>

int main(void){
	char* ip = "192.168.161.128";
	char* port = "8080";

	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	int epoll_fd = epoll_create(1);

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd  = STDIN_FILENO;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

	event.data.fd  = socket_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);

	while(1){
		// 构建就绪接收数组
		struct epoll_event events[10];

		int epoll_num = epoll_wait(epoll_fd, events, 10, -1);

		for(int i=0; i < epoll_num; i++){
			char buf[60] = {0};
			if(events[i].data.fd == STDIN_FILENO){
				read(STDIN_FILENO, buf, sizeof(buf) - 1);
				sendto(socket_fd, buf, sizeof(buf), 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
			}else if(events[i].data.fd == socket_fd){
				recvfrom(socket_fd, buf, sizeof(buf), 0, NULL, NULL);
				printf("-> %s\n", buf);
			}
		}
	}
	close(socket_fd);
	return 0;
}

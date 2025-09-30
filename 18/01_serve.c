#include <header.h>

int main(void){
	char* ip = "192.168.161.128";
	char* port = "8080";

	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);
	bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

	int epoll_fd = epoll_create(1);

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd  = STDIN_FILENO;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

	event.data.fd  = socket_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);

	//保存客户端地址
	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));

	while(1){
		// 构建就绪接收数组
		struct epoll_event events[10];

		int epoll_num = epoll_wait(epoll_fd, events, 10, -1);

		for(int i=0; i < epoll_num; i++){
			if(events[i].data.fd == STDIN_FILENO){
				char w_buf[60] = {0};
				read(STDIN_FILENO, w_buf, sizeof(w_buf) - 1);
				if(!client_addr.sin_port){
					printf("客户端未知\n");
				}else{
					sendto(socket_fd, w_buf, sizeof(w_buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
				}
			}
			if(events[i].data.fd == socket_fd){
				char buf[60] = {0};
				socklen_t sock_len = sizeof(client_addr);
				recvfrom(socket_fd, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &sock_len);
				printf("-> %s\n", buf);
			}
		}

	}
	close(socket_fd);
	return 0;
}

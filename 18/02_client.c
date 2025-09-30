#include <header.h>

int main(void){
	char *ip = "192.168.161.128";
	char *port = "8080";

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	//sockaddr 结构体
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(port));
	sockaddr.sin_addr.s_addr = inet_addr(ip);

	//connect
	int err_con = connect(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	ERROR_CHECK(err_con, -1, "connect");

	int epoll_fd = epoll_create(1);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = STDIN_FILENO;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

	event.data.fd = socket_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);

	int flag = 1;
	while(1){
		struct epoll_event events[10];
		memset(&events, 0, sizeof(events));

		int epoll_no = epoll_wait(epoll_fd, events, 10, -1);

		for(int i = 0; i < epoll_no; i++){
			if(events[i].data.fd == socket_fd){
				char r_buf[60] = {0};

				int res_recv = recv(socket_fd, r_buf, sizeof(r_buf), 0);
				ERROR_CHECK(res_recv, -1, "recv");
				if(res_recv == 0){
					printf("连接断开，会话终止\n");
					flag = 0;
					break;
				}

				printf("-> %s\n", r_buf);
			}

			if(events[i].data.fd == STDIN_FILENO){
				char w_buf[60] = {0};

				int res_read = read(STDIN_FILENO, w_buf, sizeof(w_buf) - 1);
				ERROR_CHECK(res_read, -1, "read");
				if(res_read == 0){//EOF字符
					continue;
				}

				int res_send = send(socket_fd, w_buf, res_read, 0);
				ERROR_CHECK(res_send, -1, "send");
			}
		}
		if (!flag){
			break;
		}
	}
	close(socket_fd);
	return 0;
}

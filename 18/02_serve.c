#include <header.h>

typedef struct{
	int net_fd;
	int alive;
	time_t  t;
}Conn;

int main(void){
	char* port = "8080";
	char* ip = "192.168.161.128";

	//socket
	int socket_fd  = socket(AF_INET, SOCK_STREAM, 0);

	int reuse = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

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

	Conn list[20];
	bzero(&list, sizeof(list));
	int size = 0;

	int epoll_fd = epoll_create(1);
	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = socket_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);

	while(1){
		struct epoll_event events[10];
		memset(&events, 0, sizeof(events));

		int epoll_no = epoll_wait(epoll_fd, events, 10, 1000);

		for(int i = 0;i < epoll_no; i++){
			int curr_fd = events[i].data.fd;

			if(curr_fd == socket_fd){
				int new_fd = accept(socket_fd, NULL, NULL);

				list[size].net_fd = new_fd;
				list[size].alive = 1;
				list[size].t = time(NULL);
				size++;

				event.events = EPOLLIN;
				event.data.fd = new_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &event);
			}

			for(int j = 0; j < size; j++){
				char buf[60] = {0};
				if(list[j].net_fd == curr_fd && list[j].alive){
					int res_recv = recv(curr_fd, buf, sizeof(buf), 0);
					ERROR_CHECK(res_recv, -1, "recv");
					
					if(res_recv == 0){
						epoll_ctl(epoll_no, EPOLL_CTL_DEL, curr_fd, NULL);

						list[j].alive = 0;
						close(curr_fd);
						break;
					}

					//群发
					for(int k = 0; k < size; k++){
						if(list[k].alive && list[k].net_fd != list[j].net_fd){
							int res_send = send(list[k].net_fd, buf, sizeof(buf), 0);
							ERROR_CHECK(res_send, -1, "send");
						}
					}
					list[j].t = time(NULL);
					break;
				}
			}
		}
		//超时剔除
		time_t ti = time(NULL);
		for(int i = 0; i < size; i++){
			if(list[i].alive && ti > list[i].t + 30){
				int res_send = send(list[i].net_fd, "Time out", 8, 0);
				ERROR_CHECK(res_send, -1, "send");

				epoll_ctl(epoll_no, EPOLL_CTL_DEL, list[i].net_fd, NULL);
				list[i].alive = 0;
				close(list[i].net_fd);
			}
		}
	}
	close(socket_fd);
	return 0;
}

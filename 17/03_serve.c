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

	fd_set base_set;
	FD_ZERO(&base_set);
	FD_SET(socket_fd, &base_set);

	while(1){
		fd_set set;
		FD_ZERO(&set);
		memcpy(&set, &base_set, sizeof(base_set));

		struct timeval tval = {1, 0};
		select(10, &set, NULL, NULL, &tval);

		if(FD_ISSET(socket_fd, &set)){
			int new_fd = accept(socket_fd, NULL, NULL);

			list[size].net_fd = new_fd;
			list[size].alive = 1;
			list[size].t = time(NULL);
			size++;

			FD_SET(new_fd, &base_set);
		}

		for(int i = 0;i < size; i++){
			char buf[60] = {0};
			if(FD_ISSET(list[i].net_fd, &set) && list[i].alive){

				int res_recv = recv(list[i].net_fd, buf, sizeof(buf), 0);
				ERROR_CHECK(res_recv, -1, "recv");

				if(res_recv == 0){
					//该客户端关闭连接
					FD_CLR(list[i].net_fd, &base_set);
					list[i].alive = 0;
					close(list[i].net_fd);
					continue;
				}

				for(int j = 0; j < size; j++){
					if(list[j].alive && list[j].net_fd != list[i].net_fd){
						int res_send = send(list[j].net_fd, buf, sizeof(buf), 0);
						ERROR_CHECK(res_send, -1, "send");
					}
				}
				list[i].t = time(NULL);
			}
		}
		time_t ti = time(NULL);
		for(int i = 0; i < size; i++){
			if(list[i].alive && ti > list[i].t + 30){
				int res_send = send(list[i].net_fd, "Time out", 8, 0);
				ERROR_CHECK(res_send, -1, "send");

				FD_CLR(list[i].net_fd, &base_set);
				list[i].alive = 0;
				close(list[i].net_fd);

			}
		}
	}
	close(socket_fd);

	return 0;
}

#include "head.h"

int main(void){
	//创建进程池
	worker_status_t workers[4];
	memset(workers, 0, sizeof(workers));
	initProcessPool(workers, 4);

	//监听端口（TCP）
	int socket_fd;
	initTcpSocket(&socket_fd, "192.168.161.128", "8080");

	//epoll
	int epoll_fd = epoll_create(1);
	epoll_addfd(epoll_fd, socket_fd);

	for(int i = 0; i < 4; i++){
		epoll_addfd(epoll_fd, workers[i].socket_local_fd);
	}

	while(1){
		struct epoll_event events[4];
		memset(events, 0, sizeof(events));
		int epoll_num = epoll_wait(epoll_fd, events, 4, -1);

		for(int i = 0; i < epoll_num; i++){
			int fd = events[i].data.fd;
			if(fd == socket_fd){
				//新连接，交给进程池中空闲进程
				int new_fd = accept(socket_fd, NULL, NULL);

				toProcessPool_Free(new_fd, workers, 4);

				close(new_fd);
			} else {
				//进程完成任务，返回信息到主进程
				char buf[60] = {0};
				int ret_recv = recv(fd, buf, sizeof(buf), 0);
				ERROR_CHECK(ret_recv, -1, "recv");

				for(int j = 0; j < 4; j++){
					if(workers[j].socket_local_fd == fd){
						workers[j].status = FREE;
					}
				}
			}
		}
	}
	//Todo close
	//
	return 0;
}

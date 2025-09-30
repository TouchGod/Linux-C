#include "head.h"

int pipe_fd[2];


int main(void){
	pool_t ThreadPool;
	initPool(&ThreadPool, 4);

	int socket_fd;
	initTcpSocket(&socket_fd, "192.168.161.128", "8080");

	int epoll_fd = epoll_create(1);
	epoll_addfd(epoll_fd, socket_fd);

	while(1){
		struct epoll_event events[4];
		int epoll_num = epoll_wait(epoll_fd, events, 4, -1);
		for(int i = 0; i < epoll_num; i++){
			if(events[i].data.fd == socket_fd){
				int new_fd = accept(socket_fd, NULL, NULL);

				pthread_mutex_lock(&ThreadPool.lock);
				enQueue(&ThreadPool.queue, new_fd);
				pthread_cond_broadcast(&ThreadPool.cond);
				pthread_mutex_unlock(&ThreadPool.lock);
			}
		}
		
	}

	return 0;
}

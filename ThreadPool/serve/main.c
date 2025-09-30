#include "head.h"

int pipe_fd[2];
void handlder(int signo){
	write(pipe_fd[1], "1", 1);
}

int main(void){
	pipe(pipe_fd);
	if(fork() != 0){
		//父进程
		signal(2, handlder);

		wait(NULL);

		exit(0);
	}
	setpgid(0, 0);


	pool_t ThreadPool;
	initPool(&ThreadPool, 4);

	int socket_fd;
	initTcpSocket(&socket_fd, "192.168.161.128", "8080");

	int epoll_fd = epoll_create(1);
	epoll_addfd(epoll_fd, socket_fd);

	epoll_addfd(epoll_fd, pipe_fd[0]);

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
			else if(events[i].data.fd == pipe_fd[0]){
				char buf[10];
				read(pipe_fd[0], buf, 1);
				pthread_mutex_lock(&ThreadPool.lock);
				ThreadPool.exit_flag = 1;
				pthread_cond_broadcast(&ThreadPool.cond);
				pthread_mutex_unlock(&ThreadPool.lock);

				//线程退出
				for(int i = 0; i < ThreadPool.thread_num; i++){
					pthread_join(ThreadPool.thread_ids[i], NULL);
				}
				pthread_exit(NULL);
			}
		}
		
	}

	return 0;
}

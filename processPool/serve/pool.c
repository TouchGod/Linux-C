#include "head.h"

int initProcessPool(worker_status_t *p, int num){
	for(int i = 0; i < num; i++){
		int local_socket[2];
		socketpair(AF_LOCAL, SOCK_STREAM, 0, local_socket);

		pid_t pid = fork();
		ERROR_CHECK(pid, -1, "fork");
		if(pid == 0){
			close(local_socket[0]);
			//启动
			worker_main(local_socket[1]);
		}else{
			close(local_socket[1]);
			p[i].pid = pid;
			p[i].status = FREE;
			p[i].socket_local_fd = local_socket[0];
		}
	}
	return 0;
}

int toProcessPool_Free(int net_fd, worker_status_t *p, int num){
	for(int i = 0; i < num; i++){
		if(p[i].status == FREE){
			local_send(p[i].socket_local_fd, net_fd);
			p[i].status = BUSY;
			break;
		}
	}
	return 0;
}

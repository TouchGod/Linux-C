#include "head.h"

int worker_main(int socket_local_fd){
	while(1){
		// -> 1, 读取本地socket -> 等待父进程发任务过来
		int net_fd;
		local_recv(socket_local_fd, &net_fd);

		// -> 2, 和客户端通信
		do_work(net_fd);

		// -> 3, 通知父进程, 客户端服务完毕
		send(socket_local_fd, "Done!", 5, 0);
		close(net_fd);
	}

	return 0;
}

int do_work(int net_fd){
	send(net_fd, "Hello", 5, 0);

	return 0;
}

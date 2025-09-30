#include "head.h"

int worker_main(int socket_local_fd){
	while(1){
		// -> 1, 读取本地socket -> 等待父进程发任务过来
		int net_fd;
		local_recv(socket_local_fd, &net_fd);

		// -> 2, 和客户端通信
		do_work(net_fd);
		close(net_fd);

		// -> 3, 通知父进程, 客户端服务完毕
		send(socket_local_fd, "Done!", 5, 0);
	}

	return 0;
}

int do_work(int net_fd){
	//发送文件名
	char* file_name = "1.txt";
	int name_len = strlen(file_name);
	send(net_fd, &name_len, sizeof(int), 0);
	send(net_fd, file_name, name_len, 0);

	int file_fd = open(file_name, O_RDONLY);
	while(1){
		char buf[1000]={0};
		int ret = read(file_fd, buf, sizeof(buf));
		if(ret == 0){
			break;
		}
		send(net_fd, &ret, sizeof(int), 0);
		send(net_fd, buf, ret, 0);
		//sleep(1);
	}
	close(file_fd);
	return 0;
}

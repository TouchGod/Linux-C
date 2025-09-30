#include "head.h"

int do_work(int net_fd){
	char *file_name = "1.txt";
	int file_fd = open(file_name, O_RDWR);

	int name_len = strlen(file_name);
	send(net_fd, &name_len, sizeof(int), MSG_NOSIGNAL);
	send(net_fd, file_name, name_len, MSG_NOSIGNAL);

	// 获得文件信息
	struct stat stat_file;
	fstat(file_fd, &stat_file);

	// 发送文件长度
	send(net_fd, &stat_file.st_size, sizeof(off_t), MSG_NOSIGNAL);
	sendfile(net_fd, file_fd, NULL, stat_file.st_size);

	close(file_fd);
	return 0;
}

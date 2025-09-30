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

	fd_set read_fds;

	while(1){
		FD_ZERO(&read_fds);
		FD_SET(STDIN_FILENO, &read_fds);
		FD_SET(socket_fd, &read_fds);

		int err_select = select(socket_fd + 1, &read_fds, NULL, NULL, NULL);
		ERROR_CHECK(err_select, -1, "select");

		if(FD_ISSET(socket_fd, &read_fds)){
			char r_buf[60] = {0};

			int res_recv = recv(socket_fd, r_buf, sizeof(r_buf), 0);
			ERROR_CHECK(res_recv, -1, "recv");
			if(res_recv == 0){
				printf("连接断开，会话终止\n");
				break;
			}

			printf("-> %s\n", r_buf);
		}

		if(FD_ISSET(STDIN_FILENO, &read_fds)){
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
	close(socket_fd);
	return 0;
}

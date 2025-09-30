#include <header.h>

int main(void){
	char* port = "8080";
	char* ip = "192.168.161.128";

	//socket
	int socket_fd  = socket(AF_INET, SOCK_STREAM, 0);

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

	int new_fd = accept(socket_fd, NULL, NULL);
	ERROR_CHECK(new_fd, -1, "accept");

	fd_set set;

    while(1){
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(new_fd, &set);

        select(new_fd + 1, &set, NULL, NULL, NULL);

        if(FD_ISSET(STDIN_FILENO, &set)){
            //读取标准输入回复给客户端
            char buf[60] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            send(new_fd, buf, sizeof(buf), 0);
        }
        if(FD_ISSET(new_fd, &set)){
            // 先读取客户端发过来的数据
            char buf[60] = {0};
            int ret = recv(new_fd, buf, sizeof(buf), 0);
            if(ret == 0){
                printf("对方断开链接 \n");
                break;
            }
            printf("-> %s \n", buf);
        }
    }
	close(new_fd);
	close(socket_fd);

	return 0;
}

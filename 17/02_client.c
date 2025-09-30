#include <header.h>

int main(){

    char *port = "8080";
    char *ip = "192.168.161.128"; 

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr=inet_addr(ip);

    int res_connect = connect(socket_fd, (struct sockaddr * )&sockaddr ,sizeof(sockaddr) );
    ERROR_CHECK(res_connect, -1, "connect");

    fd_set set;

    while(1){

        // 每次都需要重置监听集合
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(socket_fd, &set);

		struct timeval tval = {1, 0};
        select(10, &set, NULL, NULL, &tval);

        if(FD_ISSET(STDIN_FILENO, &set)){
            // 客户端先向服务器发信息
            char buf[60] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            send(socket_fd, buf, sizeof(buf), 0);
        }
        if(FD_ISSET(socket_fd, &set)){
            // 读取服务器回复的信息
            char buf[60] = {0};
            ssize_t ret = recv(socket_fd, buf, sizeof(buf), 0);
            if(ret == 0){
                printf("对方终止了对话 \n");
                break;
            }
            printf("buf: %s \n", buf);
        }
    }

    close(socket_fd);
    return 0;
}

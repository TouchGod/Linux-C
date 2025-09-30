#include <header.h>

typedef struct{
	int len;
	char buf[100];
}s_file;

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

	char* file_name = "1.txt";

	s_file sf;
	memset(&sf, 0, sizeof(sf));
	sf.len = strlen(file_name);
	memcpy(sf.buf, file_name, sf.len);
	send(socket_fd, &sf, sizeof(int) + sf.len, 0);
	/* int name_len = strlen(file_name); */
	/* send(socket_fd, &name_len, sizeof(int),0); */
	/* send(socket_fd, file_name, name_len, 0); */

	int fd = open(file_name, O_RDONLY);
	char buf[100] = {0};
	int n = read(fd, buf, sizeof(buf));
	send(socket_fd, &n, sizeof(n), 0);
	send(socket_fd, buf, n, 0);

	close(fd);
	close(socket_fd);

	return 0;
}

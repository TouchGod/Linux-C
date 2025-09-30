#include <header.h>

int main(){

	char *ip = "192.168.161.128";
	char *port = "8080";

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(atoi(port));

	connect(socketfd, (struct sockaddr *)&addr, sizeof(addr));

	char buf[60] = {0};
	int res = recv(socketfd, buf, sizeof(buf), 0);
	ERROR_CHECK(res, -1, "recv");
	printf("buf: %s \n", buf);

	close(socketfd);

	return 0;
}

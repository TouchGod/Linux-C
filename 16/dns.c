#include <header.h>
#include <error.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		error(1, 0, "Usage: %s 域名", argv[0]);
	}
	struct addrinfo *info;
	getaddrinfo(argv[1], NULL, NULL, &info);

	for(struct addrinfo *p =info; p != NULL; p = p->ai_next){
		if(p->ai_family == AF_INET){
			struct sockaddr *sockaddr = p->ai_addr;
			struct sockaddr_in * sockaddrin = (struct sockaddr_in*)sockaddr;
			struct in_addr addr = sockaddrin->sin_addr;

			char *s = inet_ntoa(addr);
			printf("ip : %s\n", s);
		}
	}
	return 0;
}

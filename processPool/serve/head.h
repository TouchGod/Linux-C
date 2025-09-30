#pragma once
#include <header.h>

// 定义:一个枚举类型, 分别代表进程`忙`和`空闲`
enum{
	BUSY,FREE
};

// 定义:用来追踪进程池中, 进程的信息和状态
typedef struct {
	pid_t pid;
	int status;
	int socket_local_fd;
}worker_status_t;

// 根据指定数量初始化进程池
int initProcessPool(
            worker_status_t *p, // 记录进程池中进程状态的数组(传入传出参数) 
            int num   // 进程池中:要创建的进程个数
            );

// 根据端口和IP构建服务端的网络监听
int initTcpSocket(
            int * socket_fd,// socket的文件描述符(传入传出参数) 
            char *ip, // ip地址
            char *port // 端口
            );

// 给epoll添加要监听的文件描述符
int epoll_addfd(
          int epoll_fd, // epoll的文件描述符
          int file_fd // 要监听的文件描述符
         );

// 把监听到的一个连接交给进程池中某个空闲进程
int toProcessPool_Free(
               int net_fd, // accept获取的连接对象的文件描述符id
               worker_status_t *p, // 进程池连接数组
               int num // 进程池中进程个数
              );

// 启动一个工作进程
int worker_main(
          int socket_local_fd // 用以和父进程通信的本地socket文件描述符
         );

// 工作进程,从本地socket中读取main进程发过来的客户端连接对象
int local_recv(
          int socket_local_fd, //用以和main进程和工作进程通信的本地socket文件描述符
          int *net_fd // 客户端连接对象的文件描述符指针
         );

// main进程accept获得的客户端连接对象发给工作进程
int local_send(
           int socket_local_fd, // main进程用来和工作进程通信的本地socket文件描述符
           int net_fd // main进程accept的客户端连接对象的文件描述符
          );

// 根据客户端连接文件对象和客户端通信
int do_work(
          int net_fd // 客户端连接对象的文件描述符
         );

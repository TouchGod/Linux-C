#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <header.h>
#include "queue.h"

// 定义线程池
typedef struct pool_s{
	// 所有子线程id
	pthread_t *thread_ids;
	// 子线程的数量
	int thread_num;
	// 任务队列
	queue_t queue;
	// 锁
	pthread_mutex_t lock;
	// 条件遍历
	pthread_cond_t cond;
}pool_t;

// 根据指定数量创建线程池
int initPool(pool_t *pPool, int num);

// 定义线程的入口函数
void *threadMain(void *p);

// 初始化连接
int initTcpSocket(int *socket_fd, char *ip, char *port);

// 添加epoll监听
int epoll_addfd(int epoll_fd, int file_fd);

// 传送文件
int do_work(int net_fd);
#endif

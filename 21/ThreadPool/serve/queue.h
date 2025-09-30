#ifndef __THREADQUEUE__
#define __THREADQUEUE__

#include <header.h>

// 定义队列结点
typedef struct node_s{
       int net_fd;
       struct node_s *pNext;
}node_t;
// 定义队列
typedef struct queue_s{
       node_t *head;
       node_t *end;
       int size;
}queue_t;

int enQueue(queue_t *pQueue, int net_fd);
int deQueue(queue_t *pQueue);

#endif

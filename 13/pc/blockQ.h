#ifndef __WD_BLOCKQ_H
#define __WD_BLOCKQ_H  // 避免头文件重复包含

#include <func.h>
#define N 1024

typedef int E;

typedef struct {
    E elements[N]; // 固定长度的数组
    int front;
    int rear;     // 下一个元素添加的位置
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} BlockQ;

BlockQ* blockq_create(void);
void    blockq_destroy(BlockQ* q);

bool blockq_empty(BlockQ* q);
bool blockq_full(BlockQ* q);
void blockq_push(BlockQ* q, E val);
E blockq_pop(BlockQ* q);
E blockq_peek(BlockQ* q);

#endif

#include "head.h"

int initPool(pool_t *pPool, int num){
	pPool->thread_num = num;
	memset(&pPool->queue, 0, sizeof(pPool->queue));
	pPool->thread_ids = (pthread_t *)calloc(num, sizeof(pthread_t));
	for(int i = 0; i < num; i++){
		pthread_create(&pPool->thread_ids[i], NULL, threadMain, pPool);
	}
	pthread_mutex_init(&pPool->lock, NULL);
	pthread_cond_init(&pPool->cond, NULL);
	pPool->exit_flag = 0;

	return 0;
}

void* threadMain(void* p){
	pool_t* pool = (pool_t*)p;
	while(1){
		pthread_mutex_lock(&pool->lock);

		while(pool->queue.size <= 0 && pool->exit_flag == 0){
			pthread_cond_wait(&pool->cond, &pool->lock);
		}//size > 0
		if(pool->exit_flag){
			pthread_mutex_unlock(&pool->lock);
			printf("线程已退出\n");
			pthread_exit(NULL);
		}

		int net_fd = pool->queue.head->net_fd;
		deQueue(&pool->queue);

		pthread_mutex_unlock(&pool->lock);

		do_work(net_fd);
		close(net_fd);
	}
	return NULL;
}

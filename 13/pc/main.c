#include "blockQ.h"
#include <func.h>

typedef struct{
	pthread_t* threads;
	int num;
	BlockQ* q;
}ThreadPool;

void* start_routine(void* arg){
	ThreadPool* pool = (ThreadPool*)arg;

	pthread_t tid = pthread_self();
	for(;;){
		int task_id = blockq_pop(pool->q);
		if(task_id == -1){
			printf("0x%lx: exit.\n", tid);
			pthread_exit(NULL);//直接退出
		}
		printf("0x%lx:execue %d task.\n", tid, task_id);
		sleep(2);
		printf("0x%lx: task %d done.\n", tid, task_id);
	}

	return NULL;
}
ThreadPool* threadpool_create(int n){
	ThreadPool* tp =(ThreadPool*) malloc(sizeof(ThreadPool));
	tp->threads = (pthread_t*)malloc(n * sizeof(pthread_t));
	tp->q = blockq_create();
	tp->num = n;

	for(int i = 0;i < n; i++){
		pthread_create(tp->threads + 1, NULL, start_routine, (void*)tp);
	}
	return tp;
}

void threadpool_destroy(ThreadPool* pool){
	blockq_destroy(pool->q);
	free(pool->threads);
	free(pool);
}
int main(void)
{
	// main线程
	// 1. 创建线程池
	ThreadPool* Tpool = threadpool_create(8);
	// 2. 主线程往阻塞队列中放任务; 子线程从阻塞队列中取任务，然后执行任务
	for(int i =0; i <= 100; i++){
		blockq_push(Tpool->q, i);
	}

	sleep(10);
	// 3. 线程池优雅地退出
	for(int i = 0; i < Tpool->num; i++){
		blockq_push(Tpool->q, -1);
	}
	for(int i = 0; i < Tpool->num; i++){
		pthread_join(Tpool->threads[i], NULL);
	}
	// 4. 销毁线程池
	threadpool_destroy(Tpool);
	return 0;
}

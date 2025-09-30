#include "blockQ.h"
#include <func.h>

BlockQ* blockq_create(void){
	BlockQ* bq = (BlockQ*)malloc(sizeof(BlockQ));
	bq->front = 0;
	bq->rear = 0;
	bq->size = 0;
	pthread_mutex_init(&bq->mutex, NULL);
	pthread_cond_init(&bq->not_empty, NULL);
	pthread_cond_init(&bq->not_full, NULL);
	return bq;
}

void blockq_destroy(BlockQ* q){
	pthread_mutex_destroy(&q->mutex);
	pthread_cond_destroy(&q->not_empty);
	pthread_cond_destroy(&q->not_full);
	free(q);
}

bool blockq_empty(BlockQ* q){
	pthread_mutex_lock(&q->mutex);
	int size = q->size;
	pthread_mutex_unlock(&q->mutex);
	return size == 0;
}

bool blockq_full(BlockQ* q){
	pthread_mutex_lock(&q->mutex);
	int size = q->size;
	pthread_mutex_unlock(&q->mutex);
	return size == N;
}

void blockq_push(BlockQ* q, E val){
	pthread_mutex_lock(&q->mutex);
	while(q->size == N){
		pthread_cond_wait(&q->not_full, &q->mutex);
	}

	q->elements[q->rear] = val;
	q->rear = (q->rear + 1) % N;
	q->size++;
	//入队，唤醒等待not_empty的进程
	pthread_cond_signal(&q->not_empty);

	pthread_mutex_unlock(&q->mutex);
}

E blockq_pop(BlockQ* q){
	pthread_mutex_lock(&q->mutex);
	while(q->size == 0){
		pthread_cond_wait(&q->not_empty, &q->mutex);
	}

	E val = q->elements[q->front];
	q->front=(q->front + 1) % N;
	q->size--;

	pthread_cond_signal(&q->not_full);

	pthread_mutex_unlock(&q->mutex);
	return val;
}

E blockq_peek(BlockQ* q){
	pthread_mutex_lock(&q->mutex);
	while(q->size == 0){
		pthread_cond_wait(&q->not_empty, &q->mutex);
	}
	E val = q->elements[q->front];
	pthread_mutex_unlock(&q->mutex);
	return val;
}

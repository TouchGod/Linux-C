#include <func.h>

typedef struct {
    int id;
    int money;
    pthread_mutex_t mutex;
} Account;

Account acct1 = {1, 100, PTHREAD_MUTEX_INITIALIZER};
Account acct2 = {2, 900, PTHREAD_MUTEX_INITIALIZER};

int transfer(Account* acct1, Account* acct2, int money){
	if(acct1->id < acct2->id){
		pthread_mutex_lock(&acct1->mutex);
		pthread_mutex_lock(&acct2->mutex);
	}else{
		pthread_mutex_lock(&acct2->mutex);
		pthread_mutex_lock(&acct1->mutex);
	}

	int ret;
	if(acct1->money >= money){
		acct1->money -= money;
		acct2->money += money;
		ret = money;
	}else{
		ret = 0;
	}
	pthread_mutex_unlock(&acct1->mutex);
	pthread_mutex_unlock(&acct2->mutex);
	return ret;
}
void* start_routine1(void* arg){
	int money = (int) arg;
	int n = transfer(&acct1, &acct2, money);
	printf("acct1 transfer %d to acct2.\n", n);
	return NULL;
}

void* start_routine2(void* arg){
	int money = (int) arg;
	int n = transfer(&acct2, &acct1, money);
	printf("acct2 transfer %d to acct1.\n", n);
	return NULL;
}

int main(void) {
    // 创建两个线程：
	pthread_t pid1, pid2;
    // 其中一个线程：从acct1转账100给acct2
	pthread_create(&pid1, NULL, start_routine1, (void*)100);
    // 同时另一个线程：从acct2转账900给acct1
	pthread_create(&pid2, NULL, start_routine2, (void*)900);
    // 要求：任何情况下，都避免死锁现象！

    // 主线程等待各个子线程结束。
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	return 0;
}

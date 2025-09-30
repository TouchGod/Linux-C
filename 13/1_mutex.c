#include <func.h>

typedef struct {
	int id;
	int money;
	pthread_mutex_t mutex;
} Account;

Account acct1 = {1, 100, PTHREAD_MUTEX_INITIALIZER};

int withdraw(Account* acct, int money){
	pthread_mutex_lock(&acct->mutex);
	int ret;
	if(acct->money <= 0){
		ret = -1;
		goto end;
	}
	if(acct->money >= money){
		ret = money;
		acct->money -= money;
	}else{
		ret = acct->money;
		acct->money = 0;
	}
end:
	pthread_mutex_unlock(&acct->mutex);
	return ret;
}

void* start_routine(void* arg){
	int money =(int) arg;
	int n = withdraw(&acct1, money);
	if(n == -1){
		printf("0x%lx:withdraw failed.\n", pthread_self());
	}else{
		printf("0x%lx:withdraw %d .\n",pthread_self(), n);
	}
	return 0;
}

int main(void) {
	// 创建10个线程，同时去acct账号上取100。
	pthread_t pids[10];
	for(int i = 0; i < 10; i++){
		pthread_create(pids + i, NULL, start_routine, (void*)100);
	}
	// 要求：任何情况下，都只能有一个线程能取成功。


	// 主线程等待各个子线程结束。
	for(int i = 0; i < 10; i++){
		pthread_join(pids[i], NULL);
	}

	return 0;
}

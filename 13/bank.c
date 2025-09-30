#include <func.h>

typedef struct {
	int id;
	int balance;
	// 细粒度锁
	pthread_mutex_t mutex;
} Account;

Account acct1 = {1, 100, PTHREAD_MUTEX_INITIALIZER};

void cleanup(void* arg) {
	Account* acct = (Account*)arg;
	pthread_mutex_unlock(&acct->mutex);
}

int withdraw(Account* acct, int money) {
	pthread_mutex_lock(&acct->mutex);
	pthread_cleanup_push(cleanup, &acct1);
	// 校验
	if (acct->balance < money) {
		pthread_mutex_unlock(&acct->mutex);
		return 0;
	}
	sleep(1); // 切换
			  // 取钱
	acct->balance -= money;
	pthread_mutex_unlock(&acct->mutex);
	return money;
	pthread_cleanup_pop(0);
}
void* start_routine(void* arg) {
	int money = (int) arg;
	int n = withdraw(&acct1, money);
	printf("%lu withdraw $%d\n", pthread_self(), n);
	return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		pthread_create(threads + i, NULL, start_routine, (void*)100);
	}
	// 等待所有子线程结束
	for(int i = 0; i < 10; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}

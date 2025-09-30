#include <func.h>

void* start_routine1(void* arg){
	long long a = *(long long*)arg;
	a >>= 1;
	long long sum = 0;
	for(long long i = 1; i < a; i++){
		sum += i;
	}
	return (void*)sum;
}

void* start_routine2(void* arg){
	long long a = *(long long*)arg;
	long long sum = 0;
	for(long long i = a >> 1; i < a; i++){
		sum += i;
	}
	return (void*)sum;
}

int main(void) {
    long long n;
    printf("Please input a long long int: ");
    scanf("%lld", &n);

    // 创建两个线程
	pthread_t tid1;
	pthread_t tid2;
	int err;
    // 第一个线程对区间[1,n/2]累加
	err = pthread_create(&tid1, NULL, start_routine1, &n);
	if (err){
		error(1, err, "pthread_create Thread 1");
	}

    // 第二个线程对区间(n/2, n]累加
	err = pthread_create(&tid2, NULL, start_routine2, (void*)&n);
	if (err){
		error(1, err, "pthread_create Thread 2");
	}

    // 主线乘等待两个子线程结束，并接收它们计算的结果。
	long long* sum1;
	pthread_join(tid1, (void**)&sum1);

	long long* sum2;
	pthread_join(tid1, (void**)&sum2);
    // 输出最终结果。
	printf("The sum is: %lld\n", *sum1 + *sum2);
	return 0;
}

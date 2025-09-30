#include <func.h>

typedef struct {
    long long left;
    long long right;
} Section;

void* start_routine(void* arg) {
    Section* sec = (Section*) arg;    
    long long sum = 0;
    for (long long i = sec->left; i <= sec->right; i++) {
        sum += i;
    }
    return (void*)sum;
}

int main(void) {
    long long n;
    printf("Please input a long long int: ");
    scanf("%lld", &n);

    // 创建两个线程
    // 第一个线程对区间[1,n/2]累加
    // 第二个线程对区间(n/2, n]累加
    pthread_t tid1, tid2;
    Section sec1 = {1, n/2};
    Section sec2 = {n/2 + 1, n};

    int err;
    err = pthread_create(&tid1, NULL, start_routine, &sec1);
    if (err) {
        error(1, err, "pthread_create");
    }

    err = pthread_create(&tid2, NULL, start_routine, &sec2);
    if (err) {
        error(1, err, "pthread_create");
    }

    // 主线乘等待两个子线程结束，并接收它们计算的结果。
    // 输出最终结果。
    long long sum = 0;
    long long retval;

    err = pthread_join(tid1, (void**)&retval);
    if (err) {
        error(1, err, "pthread_join");
    }
    sum += retval;

    err = pthread_join(tid2, (void**)&retval);
    if (err) {
        error(1, err, "pthread_join");
    }
    sum += retval;

    printf("sum = %lld\n", sum);
    return 0;
}

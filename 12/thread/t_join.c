#include <func.h>

void* start_routine(void*) {
	sleep(3);
	return (void*)9527;
}

int main(int argc, char* argv[])
{
	// 创建线程
	pthread_t tid;
	int err = pthread_create(&tid, NULL, start_routine, NULL);
	if (err) {
		error(1, err, "pthread_create");
	}
	
	int retval;
	pthread_join(tid, (void**)&retval); // 阻塞，等tid线程结束
	
	printf("main thread: retval = %d\n", retval);
    return 0;
}

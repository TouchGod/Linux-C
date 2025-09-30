#include <func.h>

void handler(int signo){
	switch (signo){
	case SIGINT:
		printf("Catch signal SIGINT\n");
		break;
	case SIGQUIT:
		printf("Catch signal SIGQUIT\n");
		break;
	case SIGTSTP:
		printf("Catch signal SIGTSTP\n");
		break;
	default:
		printf("Unknown signal\n");
		break;
	}
}

int main(int argc, char* argv[]){
	sighandler_t old_handler = signal(SIGINT, handler);
	if(old_handler == SIG_ERR){
		error(1, errno, "signal SIGINT");
	}

	old_handler = signal(SIGQUIT, handler);
	if(old_handler == SIG_ERR){
		error(1, errno, "signal SIGQUIT");
	}

	old_handler = signal(SIGTSTP, handler);
	if(old_handler == SIG_ERR){
		error(1, errno, "signal SIGTSTP");
	}

	for(;;){
		sleep(5);
	}
	return 0;
}

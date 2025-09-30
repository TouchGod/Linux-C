#include <func.h>

int main()
{
	pid_t pid = fork();
	switch(pid){
	case -1:
		error(1,errno,"fork");
	case 0:
		abort();
	default:
		int status;
		wait(&status);
	#ifdef WCOREDUMP
		if(WCOREDUMP(status)){
			sleep(5);
		}
    #endif
		break;
	}
    return 0;
}


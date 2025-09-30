#include <func.h>

int main(int argc, char* argv[]){
	pid_t pid = fork();

	switch (pid){
	case -1:
		error(1, errno, "fork");
	case 0:{
		printf("Child: pid= %d, ppid= %d\n", getpid(), getppid());
		char* cwd = getcwd(NULL, 0);
		puts(cwd);
		free(cwd);

		char chwd[] = "../09";
		if(chdir(chwd)){
			error(1,errno,"chdir");
		}
		
		char* cwd1 = getcwd(NULL, 0);
		puts(cwd1);
		free(cwd1);
		break;
	}
	default:
		printf("Parent: pid = %d, child = %d\n", getpid(), pid);

		char* cwd2 = getcwd(NULL, 0);
		puts(cwd2);
		free(cwd2);

		sleep(5);

		char* cwd3 = getcwd(NULL, 0);
		puts(cwd3);
		free(cwd3);
		break;
	}
	return 0;
}

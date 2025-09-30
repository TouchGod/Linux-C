#include <func.h>

#define MAXLINE 1024
#define MAXARGS 128

void ext_parameter(char* mes, char* argv[]){
	int i = 0;
	argv[i] = strtok(mes, " \n\t");

	while(argv[i]){
		i++;
		argv[i] = strtok(NULL, " \n\t");
	}
}

void print_wstatus(int status){
	//正常退出
	if(WIFEXITED(status)){
		printf("Exit code: %d",WIFEXITED(status));
	}else if(WIFSIGNALED(status)){
		//异常终止，给出信号
		printf("Terminate signal: %d ", WIFSIGNALED(status));
		//支持core dump
#ifdef WCOREDUMP
		if(WCOREDUMP(status)){
			printf("code dump");
		}
#endif
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	char* parameter[MAXARGS];
    for(;;) {
        // 读取用户输入的命令 cmd
		printf(" > ");
		char cmd[MAXLINE];
		fgets(cmd, MAXLINE, stdin);
        // 如果cmd为exit，终止进程
		if(strcmp(cmd, "exit\n") == 0){
			exit(1);
		}

		// 创建子进程，让子进程执行命令
		pid_t pid = fork();
		switch(pid){
		case -1:
			error(1, errno, "fork");
		case 0:
			ext_parameter(cmd, parameter);
			if(execvp(parameter[0], parameter) == -1){
				error(1, errno, "execvp");
			}
			break;
		default:
			// 父进程等待子进程结束，并打印子进程的终止状态信息。
			int status;
			if(waitpid(pid, &status, 0) == -1){
				error(1, errno, "waitpid");
			}

			printf("%d terminated.\n", pid);
			print_wstatus(status);
			break;
		}
    }
	return 0;
}

#include "def.h"
#include "executor.h"

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>

/* just for debug.*/
void print_args()
{
	
	printf("-----Debug Info Start-------\n");
	int i,j;
	printf("cmds_count:[%d]\n",cmds_count);
	for(i = 0;i<cmds_count;i++)
	{
		printf("cmd[%d]:\n",i);
		for(j = 0;cmds[i].args[j] != NULL;j++)
			printf("\t[%s]\n",cmds[i].args[j]);
	}
	printf("infile:[%s]\n",infile);
	printf("outfile:[%s]\n",outfile);
	printf("-----Debug Info End  -------\n");
}


int exec_cmd()
{

#ifdef DEBUG
	print_args();
#endif

	int i;
	pid_t pid;
	int pipefd[2];

	if(cmds_count == 0)
		return 0;

	if(infile[0] != '\0')
	{
		cmds[0].infd = open(infile,O_RDONLY);
	}

	if(outfile[0] != '\0')
	{
		if(append)
			cmds[cmds_count-1].outfd = open(outfile,O_WRONLY|O_CREAT|O_APPEND,0666);
		else
			cmds[cmds_count-1].outfd = open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0666);
	}

	/* 因为后台作不会调用wait等待子进程退出.为避免僵死进程，可以忽略SIGCHLD信号 */
	if (background == 1)
		signal(SIGCHLD, SIG_IGN);
	else
		signal(SIGCHLD, SIG_DFL);
	 /*
	 有多少条命令，父进程就创建多少个子进程去执行，这些子进程之间的通过pipe通信。
	 输入重定向（<）只可能是第一条命令的。
	 输出重定向（>）只可能是最后一条命令的。
	 * */
	for(i = 0;i < cmds_count; i++)
	{
		if(i < cmds_count - 1) //不是最后一条命令，则需要创建管道.
		{
			pipe(pipefd);
			cmds[i].outfd = pipefd[1];
			cmds[i+1].infd = pipefd[0];
		}

		pid = fork();
		if(pid == -1)
			perror("fork");
		if(pid > 0)
		{
			if(background == 1)
				printf("%d\n",pid);

			lastcpid = pid;
			int fd;

			/*关闭被重定向的描述符，以便下次fork进程。 */	
			if((fd = cmds[i].infd) != 0)
				close(fd);
			if((fd = cmds[i].outfd) != 1)
				close(fd);
		}
		if(pid == 0)
		{
			/* backgnd=1时，将第一条简单命令的infd重定向至/dev/null */
			/* 当第一条命令试图从标准输入获取数据的时候立即返回EOF */
			if (cmds[i].infd == 0 && background == 1)
				cmds[i].infd = open("/dev/null", O_RDONLY);

			/* 将第一个简单命令进程作为进程组组长 */
			if (i == 0)
				setpgid(0, 0);

			if(cmds[i].infd != 0)	//输入被重定向了
				dup2(cmds[i].infd,0);
			
			if(cmds[i].outfd != 1)	//输出被重定向了
				dup2(cmds[i].outfd,1);

			int j;
			int maxfd = sysconf (_SC_OPEN_MAX);
			for(j = 3;j<= maxfd;j++)  //关闭3以后的描述符
				close(j);
			
			if(execvp(cmds[i].args[0],cmds[i].args) == -1)
				fputs("unrecognized command\n",stderr);
			exit(EXIT_FAILURE);
		}
	}

	/* 前台作业，等待所有的子进程结束. */
	if(background == 0)
		while(wait(NULL) != lastcpid);

	return 0;
}

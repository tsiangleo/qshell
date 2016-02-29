#include "def.h"
#include "executor.h"

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

/* just for debug.*/
void print_args()
{
	
	printf("-----start-------\n");
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

	/*
	printf("inputline:[%s]\n",inputline);

	printf("fmtline:[");
	for(i = 0;i<MAXLINE;i++)
	{
		printf("%c",fmtline[i] == '\0' ? '#':fmtline[i]);
	}
	printf("]\n");

	printf("infile:[");
	for(i = 0;i<MAXNAME;i++)
	{
		printf("%c",infile[i] == '\0' ? '#':infile[i]);
	}
	printf("]\n");

	printf("outfile:[");
	for(i = 0;i<MAXNAME;i++)
	{
		printf("%c",outfile[i] == '\0' ? '#':outfile[i]);
	}
	printf("]\n");
	*/
	printf("-----end ------\n");
}


int exec_cmd()
{

#ifdef DEBUG
	print_args();
#endif

#ifndef DEBUG
	int i;
	pid_t pid;
	int status;		
	int pipefd[2];

	int lastcpid;

	/*
	 *
	 有多少条命令，父进程就创建多少个子进程去执行，这些子进程之间的通过pipe通信。
	 输入重定向（<）只可能是第一条命令的。
	 输出重定向（>）只可能是最后一条命令的。
	 * */
	for(i = 0;i < cmds_count; i++)
	{
		if(i < cmds_count - 1) //不是最后一条命令
		{
			pipe(pipefd);
			cmds[i].outfd = pipefd[1];
			cmds[i+1].infd = pipefd[0];
		}

		pid = fork();

		if(pid > 0)
		{
			lastcpid = pid;

			int fd;
		/* ????????????????  */	
			if((fd =cmds[i].infd) != 0)
				close(fd);
			if((fd =cmds[i].outfd) != 1)
				close(fd);


		}

		if(pid == 0)
		{
			if(cmds[i].infd != 0)//输入被重定向了
			{
				close(0);
				dup(cmds[i].infd);
			}
			if(cmds[i].outfd != 1)//输出被重定向了
			{
				close(1);
				dup(cmds[i].outfd);

			}
			int j;
			int maxfd = sysconf (_SC_OPEN_MAX);
			for(j = 3;j<= maxfd;j++)  //关闭3以后的描述符
			{
				close(j);
			}
			execvp(cmds[i].args[0],cmds[i].args);
			exit(EXIT_FAILURE);
		}
	}

	while(wait(NULL) != lastcpid);

#endif
	/*
	pid_t pid;
	int status;

	if((pid = fork()) == -1)
	{
		ERR_EXIT("fork");
	}
	
	if(pid == 0)
	{
		execvp(argv[0],argv);
	}

	waitpid(pid,&status,0);
*/
	return 0;
}

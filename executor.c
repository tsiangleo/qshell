#include "def.h"
#include "executor.h"

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* just for debug.*/
void print_args()
{
	int i,j;
	
	printf("-----start-------\n");
	for(i = 0;i<cmds_count;i++)
	{
		printf("cmd[%d]:\n",i);
		for(j = 0;j<cmds[i].args_count;j++)
			printf("\t[%s]\n",cmds[i].args[j]);
	}
	printf("infile:[%s]\n",infile);
	printf("outfile:[%s]\n",outfile);
	printf("-----end-------\n");
}


int exec_cmd()
{
	print_args();

	/*
#ifdef DEBUG
	print_args();
#endif

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)


char inputline[1024] = {0};

/* 输出提示行信息:"username@hostname:cwd$ " */
void showhint()
{
	char hostname[512] = {0};
	char username[512] = {0};
	char cwd[1024] = {0};
	char *homedir;
	
	gethostname(hostname,sizeof(hostname));
	getlogin_r(username,sizeof(username));
	getcwd(cwd,sizeof(cwd));
	homedir = getenv("HOME");

	/* 如果目录中包含home目录，则将输出中的home目录部分替换为~. */
	if(strncmp(cwd,homedir,strlen(homedir)) == 0)
	{
		//char dir[1024] = {0};
		//dir[0] = '~';
		//strncpy(dir+1,cwd+strlen(homedir),strlen(cwd)-strlen(homedir));
		//printf("[qshell]%s@%s:%s$ ",username,hostname,dir);
		printf("%s@%s:~%s$$ ",username,hostname,cwd+strlen(homedir));
	}
	else
		printf("%s@%s:%s$$ ",username,hostname,cwd);
}

int getcmd()
{
	memset(inputline,0,sizeof(inputline));
	fgets(inputline,sizeof(inputline),stdin);
	inputline[strlen(inputline) - 1] = 0; //替换掉末尾的'\n'
	return 0;
}

#define MAXARGS 10
char *argv[MAXARGS+1]; 	
int arg_count; //解析出来的参数个数

int parsecmd()
{
	int len = strlen(inputline);
	int i;

	//将inputline中所有的空字符替换为'\0'
	for(i = 0;i<len;i++)
	{
		if(inputline[i] == ' ' || inputline[i] == '\t')
			inputline[i] = 0;
	}

	/* 将每个参数解析出来.  */
	arg_count = 0; /* 每次都要先设置为0.  */
	for(i=0;i<len;i++)
	{
		if(i == 0)
		{
			if(inputline[i] != 0)
				argv[arg_count++] = &inputline[i];
		}
		else if(inputline[i-1] == 0 && inputline[i] != 0)
		{
			argv[arg_count++] = &inputline[i];
		}
	}
	argv[arg_count] = NULL;

	return 0;
}

/* just for debug.*/
void print_args()
{
	int i;
	
	printf("-----start-------\n");
	for(i = 0;i<arg_count;i++)
	{
		printf("%s\n",argv[i]);
	}
	printf("-----end-------\n");
}

int execmd()
{

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

	return 0;
}

int main()
{
	/*
	 * 1.读取命令
	 * 2.解析命令
	 * 3.执行命令，显示结果
	 *
	 * */
	while(1)
	{
		showhint();
		getcmd();
		parsecmd();
		execmd();
	}
	return 0;
}

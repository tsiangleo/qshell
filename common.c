#include "def.h"
#include "common.h"
#include <string.h>
#include <unistd.h>
#include <signal.h>

void get_hostname()
{
	if(gethostname(hostname,sizeof(hostname)) == -1)
		perror("gethostname");
}

void get_username()
{
	if(getlogin_r(username,sizeof(username)) != 0)
		perror("getlogin_r");
}

void get_homedir()
{
	if((homedir = getenv("HOME")) == NULL)
		perror("getenv");
}

void get_cwd()
{
	if(getcwd(cwd,sizeof(cwd)) == NULL)
		perror("getcwd");
}

/* 输出提示行信息:"username@hostname:cwd$ " */
void show_hint()
{
	get_cwd();
	// 如果目录中包含home目录，则将输出中的home目录部分替换为~. 
	if(strncmp(cwd,homedir,strlen(homedir)) == 0)
		printf("%s@%s:~%s$$ ",username,hostname,cwd+strlen(homedir));
	else
		printf("%s@%s:%s$$ ",username,hostname,cwd);
	fflush(stdout);
//	printf("[qshell]$ ");
//	fflush(stdout);
}


void sigint_handler(int sig)
{
	printf("\n");
	show_hint();
}

/* 安装信号. */
void setup()
{
	signal(SIGINT,sigint_handler);
	signal(SIGQUIT,SIG_IGN);
}


void cleanup()
{
	int i;
	memset(cmds,0,sizeof(cmds));
	for(i = 0;i < MAXCMDS;i++)
	{
		cmds[i].infd = 0;
		cmds[i].outfd = 1;
	}
	memset(inputline,0,sizeof(inputline));
	memset(fmtline,0,sizeof(fmtline));
	memset(infile,0,sizeof(infile));
	memset(outfile,0,sizeof(outfile));
	memset(cwd,0,sizeof(cwd));
	fmtptr = fmtline;
	inputptr = inputline;
	background = 0;
	lastcpid = 0;
	append = 0;
	cmds_count = 0;

	show_hint();
}


/* 判断输入串中inputptr指向的当前位置是否包含str串，包含返回1,并移动inputptr指针；否则返回0.*/
int contains(char *str)
{
	while(*inputptr == ' ' || *inputptr == '\t')
		inputptr++; 	//跳过空白字符

	char *p = inputptr;

	while(*str != '\0' && *str == *p)
	{
		str++;
		p++;
	}


	if(*str == '\0')
	{
		inputptr = p;
		return 1;
	}
	return 0;
}

/* 从inputptr指向的位置开始获取文件名保存到file中.*/
void getfile(char *file)
{

	while(*inputptr == ' ' || *inputptr == '\t')
		inputptr++; 	//跳过空白字符
	
	while(*inputptr != '\0' &&
			*inputptr != '<' &&
			*inputptr != '>' &&
			*inputptr != '|' &&
			*inputptr != '&' &&
			*inputptr != ' ' &&
			*inputptr != '\t' &&
			*inputptr != '\n' )
		{
			*file++  = *inputptr++;
		}
	*file = '\0';
}


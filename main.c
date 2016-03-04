#include "def.h"
#include "init.h"
#include "parser.h"
#include "common.h"
#include "executor.h"


cmd_t	cmds[MAXCMDS];
int		cmds_count;
char 	inputline[MAXLINE+1];
char 	fmtline[MAXLINE+1];
char 	infile[MAXNAME+1];
char 	outfile[MAXNAME+1];

char *inputptr;//自动机的指针执行输入串的某个位置.
char *fmtptr;
int append;
int background;
int lastcpid;


char hostname[MAXLINE];	//主机名字
char username[MAXLINE];	//登陆名
char cwd[MAXLINE];	//当前工作目录
char *homedir;


int main()
{
	/*
	 * 1.读取命令
	 * 2.解析命令
	 * 3.执行命令，显示结果
	 *
	 * */
	init();
	while(1)
	{
		cleanup();
		if(get_cmd() == -1)
			continue;
		parse_cmd();
		exec_cmd();
	}
	return 0;
}

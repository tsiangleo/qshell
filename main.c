#include "def.h"
#include "init.h"
#include "parser.h"
#include "executor.h"


cmd_t	cmds[MAXCMDS];
int		cmds_count;
char 	inputline[MAXLINE+1];
char 	fmtline[MAXLINE+1];
char 	infile[MAXNAME+1];
char 	outfile[MAXNAME+1];

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
		init();
		get_cmd();
		parse_cmd2();
		exec_cmd();
	}
	return 0;
}

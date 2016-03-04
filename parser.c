#include "def.h"
#include "inner.h"
#include "common.h"
#include "parser.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* 获取用户输入的命令，保存到inputline中，包含\n. 成功返回0,失败返回-1.*/
int get_cmd()
{
	memset(inputline,0,sizeof(inputline));
	if(fgets(inputline,sizeof(inputline),stdin) == NULL)
		return -1;
	return 0;
}




/* 从inputptr指向的位置解析第i个命令，并保存到cmds[i].*/
void parse(int i)
{
	int argindex = 0;
	int inword = 0;
	while(*inputptr != '\0')
	{
		while(*inputptr == ' ' || *inputptr == '\t')
			inputptr++; 	//跳过空白字符
	
		cmds[i].args[argindex] = fmtptr;

		while(*inputptr != '\0' &&
				*inputptr != '<' &&
				*inputptr != '>' &&
				*inputptr != '|' &&
				*inputptr != '&' &&
				*inputptr != ' ' &&
				*inputptr != '\t' &&
				*inputptr != '\n' )
		{
			*fmtptr++  = *inputptr++;
			inword  = 1;
		}		
		*fmtptr++ = '\0';

		switch(*inputptr)
		{
			case '\t':
			case ' ':
				inword = 0;
				argindex++;
				break;
			case '<':
			case '>':
			case '|':
			case '&':
			case '\n':
				if(inword == 0)
					cmds[i].args[argindex] = NULL;
				return;
			default:
				return;
		}
	}
}




/*
 cmd [ < filename ] [ | cmd ]…[ or filename ] [ & ]
 其中方括号表示可选
 省略号(…)表示前面可以重复0次或多次
 其中or可以是>或者>>

 cat < input.txt | grep public > out.txt &

 * 解析命令.
 * 成功返回解析到的命令个数，失败返回-1
 *
 */
int parse_cmd()
{
	if(contains("\n"))
		return 0;

	/* 解析是否是内部命令. */
	if(inner())
		return 0;

	/* 解析第一条命令. */
	parse(0);
	/* 判断是否有输入重定向符.*/
	if(contains("<"))
	{
		getfile(infile);
	}
	/* 判断是否有管道.*/
	int i;
	for(i = 1;i<MAXCMDS;i++)
	{
		if(contains("|"))
		{
			parse(i);
		}
		else
			break;
	}
	/* 判断是否有输出重定向符.*/
	if(contains(">"))
	{
		if(contains(">"))
			append = 1;
		getfile(outfile);
	}

	/* 判断是否是后台作业.*/
	if(contains("&"))
	{
		background = 1;
	}

	/* 判断是否结束.*/
	if(contains("\n"))
	{
		cmds_count = i;
		return cmds_count;
	}
	else
	{
		printf("syntax error near '%c'\n",*inputptr);
		return -1;
	}
}


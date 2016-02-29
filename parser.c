#include "def.h"
#include "parser.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* 获取用户输入的命令，保存到inputline中，包含\n。*/
void get_cmd()
{
	memset(inputline,0,sizeof(inputline));
	fgets(inputline,sizeof(inputline),stdin);
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

 //0cat0<0input.txt0|0grep0public0>0out.txt0&0
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

/*
 ls -l |grep define & < def.h > tt

 ls -l |grep define < def.h > tt


 cmd [ < filename ] [ | cmd ]…[ or filename ] [ & ]
 其中方括号表示可选
 省略号(…)表示前面可以重复0次或多次
 其中or可以是>或者>>

 cat < input.txt | grep public > out.txt &

 0cat0<0input.txt0|0grep0public0>0out.txt0&0

 */
int parse_cmd()
{
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
		return 1;
	}
	else
	{
		printf("syntax error near '%c'\n",*inputptr);
		return -1;
	}
}


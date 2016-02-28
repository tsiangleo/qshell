#include "def.h"
#include "parser.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* 获取用户输入的命令，保存到inputline中，不保存换行符。*/
void get_cmd()
{
	memset(inputline,0,sizeof(inputline));
	fgets(inputline,sizeof(inputline),stdin);
	inputline[strlen(inputline) - 1] = 0; //替换掉末尾的'\n'
}


/*
 -l |grep define & < def.h > tt

 ls -l |grep define < def.h > tt


 cmd [ < filename ] [ | cmd ]…[ or filename ] [ & ]
 其中方括号表示可选
 省略号(…)表示前面可以重复0次或多次
 其中or可以是>或者>>

 cat < input.txt | grep public > out.txt &

 0cat0<0input.txt0|0grep0public0>0out.txt0&0

 */

/* 解析命令，将解析出来个命令保存到cmds数组中，命令总的条数保存到cmds_count中。成功返回0,失败返回-1. */
int parse_cmd()
{
	
	int len = strlen(inputline);
	int i;
	
	int has_input = 0;
	int has_output = 0;
	int has_background = 0;
	int cmd_index[MAXCMDS] = {0};

	if(len < 1) //空命令
		return -1;


	//将inputline中所有的空字符替换为'\0'
	cmds_count = 1; /* 每次都要先设置为0.  */
	for(i = 0;i<len;i++)
	{
		if(inputline[i] == ' ' || inputline[i] == '\t')
			inputline[i] = 0;

		if(inputline[i] == '|')
		{
			cmd_index[cmds_count++] = i; // "存放|在命令行中的下标.",存放每条命令的结束的下标位置。
		}
		if(inputline[i] == '<')
			has_input = i;
		if(inputline[i] == '>')
			has_output = i;
		if(inputline[i] == '&')
			has_background = i;
	}

	cmd_index[cmds_count] = len;//便于处理最后一条命令
	cmd_index[0] = -1;//便于处理最后一条命令

printf("cmd length:%d  cmds_count:%d\n",len,cmds_count);

/* 将每个参数解析出来.  */
	for(i=0;i<cmds_count;i++)
	{
 //			0cat0<0input.txt0|0grep0public0|0grep0hellop0>0out.txt0&0
 //			123|12345
 //			0123456789
 //
			char *pline = (i == 0 ? inputline : &inputline[cmd_index[i]] + 1);
			int wide = cmd_index[i+1] - cmd_index[i] - 1; //"两个|之间的字符串长度.“
			int arg,j;
			
			/* 含有输入重定向  */
			if(has_input > 0 && i == 0)
				wide = has_input;   /* < 距离字符串首地址的距离*/

			/* 含有输出重定向  */
			if(has_output > 0 && i == cmds_count - 1)
				wide = has_output - cmd_index[i] - 1; /* 最后一个| 和 > 之间的距离*/

			/* 解析每个命令的各个参数 */
			for(j = 0,arg = 0;j < wide; j++)
			{
				if(j == 0)
				{
					if(pline[j] != 0)
						cmds[i].args[arg++] = &pline[j];
				}
				else if(pline[j] != 0 && pline[j-1] == 0)
					cmds[i].args[arg++] = &pline[j];
			}
			cmds[i].args[arg] = NULL;
			cmds[i].args_count = arg;
	}

 //			0cat0<0input.txt0|0grep0public0|0grep0hellop0>0out.txt0&0
	if(has_input > 0 && has_input < len)
	{
		int j = 0;

		while(inputline[++has_input] == 0);
		//此时has_input指向一个不为0的字符。
		while(inputline[has_input] != 0)
			infile[j++] = inputline[has_input++];
		infile[j] = 0;
	}
	
	if(has_output > 0 && has_output < len)
	{
		int j = 0;
		
		while(inputline[++has_output] == 0);
		while(inputline[has_output] != 0)
			outfile[j++] = inputline[has_output++];
		outfile[j] = 0;
	}

	return 0;
}



/* 预处理用户的输入行，保存到fmtline中.fmtline中的各个命令之间以及各个参数之间是以'\0'隔开的,且首尾不含空格. */

/* 不管用户以那种方式输入，最后统一成一种格式。 */
int fmt_inputline()
{
	int len = strlen(inputline);
	int i = 0;
	int fmt_len = 0;

/*
 * 	00ls<in.txt|wc -c>out.txt& 00
	ls<def.h|wc -c>out.txt&
	ls < def.h | wc -c > out.txt
 * */

	while(inputline[i] == ' ' || inputline[i] == '\t')
		i++; 	//跳过空白字符
	
	for(;i < len; i++)
	{
		if(inputline[i] == '<' || inputline[i] == '>' || inputline[i] == '|' || inputline[i] == '&')
		{
			fmtline[fmt_len++] = '\0';
			fmtline[fmt_len++] = inputline[i];
			fmtline[fmt_len++] = '\0';
		}
		else
			fmtline[fmt_len++] = inputline[i];
	}

	printf("len:%d,fmt_len:%d, fmtline:[",len,fmt_len);
		for(i = 0;i<fmt_len;i++)
		printf("%c",fmtline[i] == '\0'? ' ': fmtline[i]);
	printf("]\n");
	
	return fmt_len;
}

/* 解析命令，将解析出来个命令保存到cmds数组中，命令总的条数保存到cmds_count中。成功返回0,失败返回-1. */
int parse_cmd2()
{
	
	int len = fmt_inputline();
	int i;
	
	int has_input = 0;
	int has_output = 0;
	int has_background = 0;
	int cmd_index[MAXCMDS] = {0};

	if(len < 1) //空命令
		return -1;

	cmds_count = 1; /* 每次都要先设置为0.  */
	for(i = 0;i<len;i++)
	{
		if(fmtline[i] == '|')
		{
			cmd_index[cmds_count++] = i; // "存放|在命令行中的下标.",存放每条命令的结束的下标位置。
		}
		if(fmtline[i] == '<')
			has_input = i;
		if(fmtline[i] == '>')
			has_output = i;
		if(fmtline[i] == '&')
			has_background = i;
	}

	cmd_index[cmds_count] = len;//便于处理最后一条命令
	cmd_index[0] = -1;//便于处理最后一条命令

printf("cmd length:%d  cmds_count:%d\n",len,cmds_count);

/* 将每个参数解析出来.  */
	for(i=0;i<cmds_count;i++)
	{
 //			0cat0<0input.txt0|0grep0public0|0grep0hellop0>0out.txt0&0
 //			123|12345
 //			0123456789
 //
			char *pline = (i == 0 ? fmtline : &fmtline[cmd_index[i]] + 1);
			int wide = cmd_index[i+1] - cmd_index[i] - 1; //"两个|之间的字符串长度.“
			int arg,j;
			
			/* 含有输入重定向  */
			if(has_input > 0 && i == 0)
				wide = has_input;   /* < 距离字符串首地址的距离*/

			/* 含有输出重定向  */
			if(has_output > 0 && i == cmds_count - 1)
				wide = has_output - cmd_index[i] - 1; /* 最后一个| 和 > 之间的距离*/

			/* 解析每个命令的各个参数 */
			for(j = 0,arg = 0;j < wide; j++)
			{
				if(j == 0)
				{
					if(pline[j] != 0)
						cmds[i].args[arg++] = &pline[j];
				}
				else if(pline[j] != 0 && pline[j-1] == 0)
					cmds[i].args[arg++] = &pline[j];
			}
			cmds[i].args[arg] = NULL;
			cmds[i].args_count = arg;
	}

 //			0cat0<0input.txt0|0grep0public0|0grep0hellop0>0out.txt0&0
	if(has_input > 0 && has_input < len)
	{
		int j = 0;

		while(fmtline[++has_input] == 0);
		//此时has_input指向一个不为0的字符。
		while(fmtline[has_input] != 0)
			infile[j++] = fmtline[has_input++];
		infile[j] = 0;
	}
	
	if(has_output > 0 && has_output < len)
	{
		int j = 0;
		
		while(fmtline[++has_output] == 0);
		while(fmtline[has_output] != 0)
			outfile[j++] = fmtline[has_output++];
		outfile[j] = 0;
	}

	return 0;
}


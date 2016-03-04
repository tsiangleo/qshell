#ifndef _DEF_H_
#define _DEF_H_

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
	do \
	{ \
    	perror(m); \
		exit(EXIT_FAILURE); \
  	} \
  	while (0)


#define MAXLINE 1024	/* 输入行的最大长度 */
#define MAXARGS 20		/* 每个简单命令的参数最多个数 */
#define MAXCMDS 5		/* 一个管道行中简单命令的最多个数 */
#define MAXNAME 100		/* IO重定向文件名的最大长度 */

typedef struct command
{
	char *args[MAXARGS+1];	/* 解析出的命令参数列表 */
	int args_count;
	int infd;
	int outfd;
} cmd_t;


/* 命令数组. */
extern	cmd_t	cmds[MAXCMDS];

/* 输入的命令个数. */
extern	int		cmds_count;

/* 用户输入的内容. */
extern	char 	inputline[MAXLINE+1];

/* 输入串格式化后的内容. */
extern char fmtline[MAXLINE+1];

/* 输入重定向文件名. */
extern	char 	infile[MAXNAME+1];

/* 输出重定向文件名. */
extern	char 	outfile[MAXNAME+1];

/* 表示当前处理到输入串的那个位置了. */
extern char *inputptr;

/* 表示格式化的位置. */
extern char *fmtptr;

/* 标识输出重定向是否是以追加的方式. */
extern int append;

/* 标识是否是后台作业. */
extern int background;

/* 当有多个管道时，lastcpid用来记录执行最后一条命令的子进程id. */
extern int lastcpid;

/* 主机名字. */
extern char hostname[MAXLINE];

/* 登陆名. */
extern char username[MAXLINE];

/* home目录. */
extern char *homedir;

/* 当前工作目录. */
extern char cwd[MAXLINE];


#endif /* _DEF_H_ */

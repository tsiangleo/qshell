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
//	char *infile;
//	char *outfile;

	int infd;
	int outfd;
} cmd_t;


/* 命令数组. */
extern	cmd_t	cmds[MAXCMDS];
/* 输入的命令个数. */
extern	int		cmds_count;
/* 用户输入的内容. */
extern	char 	inputline[MAXLINE+1];

extern char fmtline[MAXLINE+1];

/* 输入重定向文件名. */
extern	char 	infile[MAXNAME+1];
/* 输出重定向文件名. */
extern	char 	outfile[MAXNAME+1];


#endif /* _DEF_H_ */

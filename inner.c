#include "def.h"
#include "inner.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef void (*handler_t)(void);
typedef struct inner_cmd
{
	char *name;
	handler_t handle;
} inner_cmd_t;


void handler_exit();
void handler_cd();
void handler_help();

inner_cmd_t innercmds[] =
{
	{"exit",handler_exit},
	{"cd",handler_cd},
	{"help",handler_help},
	{NULL,NULL},
};

void handler_exit()
{
	printf("exit\n");		
	exit(EXIT_FAILURE);
}

/*返回到上层目录 */
void cwd_parent()
{
	if(chdir("..") == -1)
		perror("chdir");
}

/* 返回到home目录.*/
void cwd_home()
{
	if(chdir(homedir) == -1)
		perror("chdir");

}

void cwd_to(char *path)
{
	if(chdir(path) == -1)
		perror("chdir");
}

void handler_cd()
{
	char path[MAXLINE] = {0};
	getfile(path);
	if(strcmp("..",path) == 0)
		cwd_parent();
	else if(strcmp(".",path) == 0)
		;
	else if(strcmp("",path) == 0 || strcmp("-",path) == 0)
		cwd_home();
	else
		cwd_to(path);
}

void handler_help()
{
	int i;

	printf("目前支持的内部命令如下:\n");
	for(i = 0;innercmds[i].name != NULL;i++)
		printf("%s\n",innercmds[i].name);

}

/* 解析并执行内部命令.返回1表示内部命令，否则返回0. */
int inner()
{
	int i = 0;
	int found = 0;
	for(;innercmds[i].name != NULL;i++)
	{
		if(contains(innercmds[i].name))
		{
			innercmds[i].handle();
			found = 1;
			break;
		}
	}
	return found;
}

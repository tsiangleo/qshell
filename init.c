#include "def.h"
#include <string.h>
#include <unistd.h>

/* 输出提示行信息:"username@hostname:cwd$ " */
void show_hint()
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


void init()
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

	show_hint();
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


char inputline[1024] = {0};

/* 输出提示行信息:"username@hostname:cwd$ " */
void showhint()
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

int getcmd()
{

	fgets(inputline,sizeof(inputline),stdin);
	inputline[strlen(inputline) - 1] = 0; //替换掉末尾的'\n'
	return 0;
}

int parsecmd()
{
	return 0;
}

int execmd()
{
	printf("%s\n",inputline);
	return 0;
}

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
		showhint();
		getcmd();
		parsecmd();
		execmd();
	}
	return 0;
}

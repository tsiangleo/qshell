#ifndef _COMMON_H_
#define _COMMON_H_

/* 判断输入串中inputptr指向的当前位置是否包含str串，包含返回1,并移动inputptr指针；否则返回0.*/
int contains(char *str);

/* 从inputptr指向的位置开始获取文件名保存到file中.*/
void getfile(char *file);

/* 打印调试信息. */
void print_args();

/* 信号安装函数. */
void setup();

/* 执行每条命令前对环境进行清理. */
void cleanup();

/* 获取主机名. */
void get_hostname();

/* 获取登陆名. */
void get_username();

/* 获取主HOME目录. */
void get_homedir();

/* 获取当前工作目录. */
void get_cwd();



#endif /* _COMMON_H_ */

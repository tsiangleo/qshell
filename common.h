#ifndef _COMMON_H_
#define _COMMON_H_

/* 判断输入串中inputptr指向的当前位置是否包含str串，包含返回1,并移动inputptr指针；否则返回0.*/
int contains(char *str);

/* 从inputptr指向的位置开始获取文件名保存到file中.*/
void getfile(char *file);

void print_args();

void setup();

void cleanup();

void get_hostname();

void get_username();

void get_homedir();

void get_cwd();



#endif /* _COMMON_H_ */

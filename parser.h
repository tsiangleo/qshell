#ifndef _PARSER_H_
#define _PARSER_H_

/* 获取用户输入的命令，保存到inputline中，包含\n. 成功返回0,失败返回-1.*/
int get_cmd();

/* 解析命令，将解析出来个命令保存到cmds数组中，命令总的条数保存到cmds_count中。成功返回0,失败返回-1. */
int parse_cmd();

#endif /*_PARSER_H_ */


/*  $Id: proto.h,v 1.10 2017/05/31 20:00:38 carpend3 Exp $  */
char** arg_parse(char* line, int *argcp);//prototype for arg_parse
int check_builtin(char** argv, int *argcp, int outfd, int errfd);//prototype for check_builtin
int expand(char *orig, char *new, int newsize, int cinfd, int cerrfd);//prototype for expand
int checkComment(int start, char *line, char c);//prototype for function that checks for # out ""
int processline (char *line, int infd, int outfd, int cerrfd, int waitFlag);
extern int mainargcp;//global var for arg count in main
extern char **mainargvp;//gloabl var for arg array in main
extern int shiftFactor;//global variable for shift and unshift
extern int exitVal;//global for exit value
extern int sigintR;//global for if the user sent a SIGINT

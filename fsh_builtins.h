#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

int fsh_cd(char **args);
int fsh_help(char **args);
int fsh_exit(char **args);
int fsh_ls(char **args);
int fsh_mkdir(char **args);
int fsh_num_builtins();
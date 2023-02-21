#include "fsh_builtins.h"

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "ls",
    "mkdir" 
};

int (*builtin_func[]) (char **) = {
    &fsh_cd,
    &fsh_help,
    &fsh_exit,
    &fsh_ls,
    &fsh_mkdir
};

int fsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int fsh_mkdir(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "fsh: expected argument to \"mkdir\"\n");
    }
    if ((mkdir(args[1], 0777)) == -1) {
        perror("fsh");
    }
    return 1;
}

int fsh_ls(char **args) {
    DIR *directory;
    struct dirent *entry;
    directory = opendir(".");
    if (!directory) {
        fprintf(stderr, "fsh: directory error");
    }
    else {
        while ((entry = readdir(directory)) != NULL) {
            if (entry->d_name[0] != '.') {
                printf("%s\n", entry->d_name);
            }     
        }
    }
    closedir(directory);
    

    return 1;
}

int fsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "fsh: expected argument to \"cd\"\n");
    }
    else {
        if (chdir(args[1]) != 0) {
            perror("fsh");
        }
    }
    return 1;
}

int fsh_help(char **args) {
    printf("flexo shell FSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (int i = 0; i < fsh_num_builtins(); ++i) {
        printf(" %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int fsh_exit(char **args) {
    return 0;
}
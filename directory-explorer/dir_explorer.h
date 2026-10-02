#ifndef DIR_EXPLORER_H  
#define DIR_EXPLORER_H 
	     
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define EOL 1
#define ARG 2
#define SEMICOLON 3

#define MAXARG 512
#define MAXBUF 512

#define COLOR_RESET "\x1b[0m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m"

struct file_struct {
    char permissions[11];  
    struct passwd *pw;
    struct group *gr;
    struct tm *tm_info;
    char time_str[20];
    char d_name[80];
    long n_link;
    long long file_size;
};

void run();

#endif

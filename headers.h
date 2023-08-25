#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <ctype.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h> 
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAX_HISTORY 10
struct bg_process
{
    char **agrv;
    pid_t pid;
    int number_of_args;
};
void processes(char* command, int i, char** args, char* home_dir, bool background_process);
#endif
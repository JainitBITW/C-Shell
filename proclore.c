#include "headers.h"
#include "proclore.h"
#include "prompt.h"

bool startswith(char *str1, char *str2)
{
    int len1 = strlen(str1), len2 = strlen(str2);
    if (len1 < len2)
        return false;
    for (int x = 0; x < len2; x++)
    {
        if (str1[x] != str2[x])
            return false;
    }
    return true;
}


void proclore(pid_t pid, char* home_dir)
{
    char path[256];
    char status[256];
    char *processStatus;
    char exePath[256];
    char memPath[256];

    long vmSize = 0;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *statusFile = fopen(path, "r");

    if (statusFile == NULL)
    {
        perror("Error opening status file" );
        exit(EXIT_FAILURE);
    }
    while (fgets(status, sizeof(status), statusFile) != NULL)
    {
        if (strstr(status, "State:") == status)
        {
            processStatus = status + 7;
            break;
        }
    }
    fclose(statusFile);
    char *statusSymbol = processStatus;
    if (processStatus[1] == '+')
    {
        processStatus[1] = '\0'; // Remove '+' for printing
    }
    else
    {
        statusSymbol = NULL;
    }
   
    printf( "pid : %d\n" , pid);
    printf( "Process Status : %c%s\n" , processStatus[0], (statusSymbol ? "+" : ""));
    printf( "Process Group : %d\n" , getpgid(pid));
    snprintf(memPath, sizeof(memPath), "/proc/%d/status", pid);
    FILE *memFile = fopen(memPath, "r");

    if (memFile == NULL)
    {
        perror("Error opening memory file");
        exit(EXIT_FAILURE);
    }

    while (fgets(status, sizeof(status), memFile))
    {
        if (strstr(status, "VmSize:") == status)
        {
            sscanf(status, "VmSize:\t%ld", &vmSize);
            break;
        }
    }

    fclose(memFile);

    // Obtain executable path
    char* new_path= (char*)calloc(1000, sizeof(char));
    sprintf(new_path, "/proc/%d/exe", pid);
    ssize_t exePathSize = readlink(new_path, exePath, sizeof(exePath) - 1);
    if (exePathSize != -1)
    {
        exePath[exePathSize] = '\0';
    }
    else
    {
        perror( "Error reading executable path" );
        exit(EXIT_FAILURE);
    }
    // convert excecutable path to relative path and use ~ for initial directory
    
    if (strstr(exePath, home_dir) == exePath)
    {
        char *temp = exePath + strlen(home_dir);
        exePath[0] = '~';
        exePath[1] = '\0';
        strcat(exePath, temp);
    }
    
    printf( "Virtual memory : %ld\n" , vmSize);
    printf( "Executable path : %s\n" , exePath);
}

#include "headers.h"

char home_dir[1000]; 



int main()
{
    // Keep accepting commands
    getcwd(home_dir, sizeof(home_dir));
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(home_dir );
        char input[4096];
        char * return_value =  fgets(input, 4096, stdin);
        if (return_value == NULL)
        {
            printf("\n");
            exit(0);
        }
        
    
        

    }
}

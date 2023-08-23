#include "headers.h"
#include "pastevents.h"
#include "peek.h"
#include "prompt.h"
#include "warp.h"
void sigintHandler(int signum)
{
	printf("\nCtrl+C (SIGINT) received. Cleaning up and exiting...\n");
	add_pastevent("exit");
	exit(signum);
}

void execute(char* command,
			 char* args[],
			 int i,
			 char* pwd,
			 char* home_dir,
			 char* previous_directory,
			 char* temp_directory)
{

	if(strcmp(command, "warp") == 0)
	{
		char* warp_argument;
		if(i == 0)
		{
			warp_argument = "";
			strcpy(temp_directory, pwd);
			warp(pwd, home_dir, warp_argument, previous_directory);
			strcpy(previous_directory, temp_directory);
		}
		else if(i == 1)
		{
			warp_argument = args[0];
			strcpy(temp_directory, pwd);
			warp(pwd, home_dir, warp_argument, previous_directory);
			strcpy(previous_directory, temp_directory);
		}
		else
		{
			for(int j = 0; j < i; j++)
			{
				warp_argument = args[j];
				strcpy(temp_directory, pwd);
				warp(pwd, home_dir, warp_argument, previous_directory);
				strcpy(previous_directory, temp_directory);
			}
		}
	}

	if(strcmp(command, "peek") == 0)
	{

		int flag_a = 0, flag_b = 0;
		char* peek_argument = calloc(1000, sizeof(char));
		
		for(int x = 0; x < i; x++)
		{
			char* ag = args[x];
			if(ag[0] == '-')
			{
				int ag_len = strlen(ag);
				for(int y = 1; y < ag_len; y++)
				{
					if(ag[y] == 'a')
						flag_a = 1;
					else if(ag[y] == 'l')
						flag_b = 1;
					else
					{

						perror("Invalid flag");
					}
				}
			}
			else
				strcpy(peek_argument, args[x]);
		}
		if(strlen(peek_argument) == 0)
		{
			strcpy(peek_argument, ".");
		}
		
		peek(home_dir, flag_a, flag_b, peek_argument);
        //free(peek_argument);

        
	}
    if (strcmp(command, "exit") ==0 )
    {
        // add_pastevent(command); already added for it in the main function 
        free(previous_directory);
        free(temp_directory);
    
        exit(0);
    }
    return ; 
}

int main()
{
	signal(SIGINT, sigintHandler);
	char home_dir[10000], pwd[10000];
	char* previous_directory = calloc(10000, sizeof(char));
	char* temp_directory = calloc(10000, sizeof(char));
	// Keep accepting commands
	getcwd(home_dir, sizeof(home_dir));
	strcpy(previous_directory, home_dir);
	getcwd(pwd, sizeof(pwd));

	while(1)
	{
		// Print appropriate prompt with username, systemname and directory before accepting input

		prompt(pwd, home_dir);
		setbuf(stdout, NULL);
		char* input = calloc(4096, sizeof(char));
		char* return_value = fgets(input, 4096, stdin);

		if(return_value == NULL)
		{
			
			printf("\nCtrl+C (SIGINT) received. Cleaning up and exiting...\n");
			add_pastevent("exit");
			exit(0);
		}
		char* each_command;
		char *commandpointer, *tokenpointer;
		input = strtok(input, "\n");
        add_pastevent(input);
		each_command = strtok_r(input, ";&", &commandpointer);

		while(each_command != NULL)
		{
			char* token = strtok_r(each_command, " \t", &tokenpointer);
			char* command = token;
			char* args[100];
			int i = 0;
			token = strtok_r(NULL, " \t", &tokenpointer);
			while(token != NULL)
			{
				args[i] = token;
				i++;
				token = strtok_r(NULL, " \t", &tokenpointer);
			}
			args[i] = NULL;
			execute(command, args, i, pwd, home_dir, previous_directory, temp_directory);
			each_command = strtok_r(NULL, ";&", &commandpointer);
		}
		getcwd(pwd, sizeof(pwd));
		free(input);
	}
	free(previous_directory);

	return 0;
}

#include "headers.h"

char home_dir[1000];

int main()
{
	// Keep accepting commands
	getcwd(home_dir, sizeof(home_dir));
	while(1)
	{
		// Print appropriate prompt with username, systemname and directory before accepting input
		prompt(home_dir);
		setbuf(stdout, NULL);
		char input[4096];
		char* return_value = fgets(input, 4096, stdin);

		if(return_value == NULL)
		{
			printf("\n");
			exit(0);
		}
		char* each_command;
		char *commandpointer, *tokenpointer;


		each_command = strtok_r(input, ";&" , &commandpointer);

		while(each_command != NULL)
		{
			char* token = strtok_r(each_command, " \t" , &tokenpointer);
			// printf("%s This is the command\n", token);
			char* command = token;
			char* args[100];
			int i = 0;
			token = strtok_r(NULL, " \t" , &tokenpointer);
			while(token != NULL)
			{
				args[i] = token;
				i++;
				token = strtok_r(NULL, " " , &tokenpointer);
			}
			
			each_command = strtok_r(NULL, ";&" , &commandpointer);

		}
	}
	return 0;
}

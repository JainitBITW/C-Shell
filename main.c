#include "headers.h"
#include "pastevents.h"
#include "peek.h"

#include "prompt.h"
#include "warp.h"
struct bg_process* bg_jobs;
bool restart_loop;
int proc_no;

void sigintHandler()
{
	// printf("\nCtrl+C (SIGINT) received. Cleaning up and exiting...\n");
	// add_pastevent("exit", home_dir );
	exit(0);
}

void execute(char* input,
			 char* command,
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

	else if(strcmp(command, "peek") == 0)
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
	else if(strcmp(command, "exit") == 0)
	{
		// add_pastevent(comman, d); already added for it in the main function
		free(previous_directory);
		free(temp_directory);
		printf("Exiting...\n");

		exit(0);
	}
	else
	{
		processes(command, i, args, home_dir, false);
	}

	return;
}
void handle_pastevents(char* input,
					   char* command,
					   char* args[],
					   int i,
					   char* pwd,
					   char* home_dir,
					   char* previous_directory,
					   char* temp_directory)

{
	if(strcmp(command, "pastevents") == 0)
	{
		if(i == 0)
		{
			print_pastevents();
		}
		else if(i == 1 && (strcmp(args[0], "purge") == 0))
		{
			pastevent_purge();
		}
		else if(i == 2 && (strcmp(args[0], "execute") == 0))
		{
			int command_number = atoi(args[1]);
			char* command_to_be_executed = give_command(command_number);
			if(command_to_be_executed == NULL)
			{
				perror("Error executing command");
				return;
			}
			char *commandpointer, *tokenpointer;
			char* each_command = strtok_r(command_to_be_executed, ";&", &commandpointer);
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
				execute(command_to_be_executed,
						command,
						args,
						i,
						pwd,
						home_dir,
						previous_directory,
						temp_directory);
				each_command = strtok_r(NULL, ";&", &commandpointer);
			}
			free(command_to_be_executed);
		}
	}
}

void processes(char* command, int i, char** args, char* home_dir, bool background_process)
{

	fflush(stdin);
	char* exec_arguments[i + 2];

	if(command[0] == '~')
	{
		exec_arguments[0] = calloc(strlen(command) + strlen(home_dir) + 20, sizeof(char));
		// strcpy(exec_arguments[0], ".");
		strcpy(exec_arguments[0], home_dir);
		strcat(exec_arguments[0], command + 1);
	}
	else
	{
		exec_arguments[0] = calloc(strlen(command) + 2, sizeof(char));
		strcpy(exec_arguments[0], command);
	}
	for(int x = 0; x < i; x++)
	{
		if(args[x][0] == '~')
		{
			exec_arguments[x + 1] = calloc(strlen(args[x]) + strlen(home_dir) + 20, sizeof(char));
			// printf("%s", args[x]);
			strcpy(exec_arguments[x + 1], home_dir);
			strcat(exec_arguments[x + 1], args[x] + 1);
		}

		else
		{
			exec_arguments[x + 1] = calloc(strlen(args[x]) + 5, sizeof(char));
			// printf("%s", args[x]);
			strcpy(exec_arguments[x + 1], args[x]);
		}
	}
	exec_arguments[i + 1] = (char*)NULL;

	int pid = fork();
	if(pid < 0)
	{
		perror("Error running process");
		return;
	}
	else if(pid == 0)
	{
		int set_pgid_error = setpgid(0, 0);
		if(set_pgid_error == -1)
		{
			perror("Error in setting pgid");
			return;
		}
		// printf("%s\n", command);
		// printf("%s", exec_arguments[1]);
		// printf("%s", exec_arguments[0]);
		if(strcmp(exec_arguments[0], "cd") == 0)
		{
			for(int x = 1; x < i + 1; x++)
			{
				int cd_status = chdir(exec_arguments[i]);
				if(cd_status < 0)
				{
					perror("Error in changing directory");
					return;
				}
			}
			return;
		}
		else
		{
			int exec_return = execvp(exec_arguments[0], exec_arguments);

			if(exec_return < 0)
			{
				perror("Error in process");
				printf("%s\n", exec_arguments[0]);
				int kill_stats = kill(getpid(), SIGTERM);
				if(kill_stats < 0)
					kill(getpid(), SIGKILL);
			}
		}
	}
	else
	{
		if(background_process)
		{
			bg_jobs[proc_no].agrv = calloc(i + 4, sizeof(char*));
			for(int y = 0; y < i + 1; y++)
			{
				bg_jobs[proc_no].agrv[y] = calloc(strlen(exec_arguments[y]) + 10, sizeof(char));
				strcpy(bg_jobs[proc_no].agrv[y], exec_arguments[y]);
			}
			bg_jobs[proc_no].agrv[i + 2] = NULL;
			bg_jobs[proc_no].pid = pid;
			bg_jobs[proc_no].number_of_args = i + 1;
			proc_no++;
			printf("%d\n", pid);
		}
		else
		{
			signal(SIGTTOU, SIG_IGN);
			int change_grp_stats = tcsetpgrp(STDIN_FILENO, pid);
			int status;
			if(change_grp_stats < 0)
			{
				perror("Error turning the process into a foreground one.");
				int k_stat = kill(getpid(), SIGTERM);
				if(k_stat < 0)
					kill(getpid(), SIGKILL);
			}
			pid_t wtpid = waitpid(pid, &status, WUNTRACED);
			// printf("\n%d\n", getpgrp());
			tcsetpgrp(STDIN_FILENO, getpgrp());
			signal(SIGTTOU, SIG_DFL);
			printf("\n");
		}
	}
	for(int y = 0; y < i + 1; y++)
		free(exec_arguments[y]);
	return;
}

void finish_proc()
{
	int status;
	bool flag = false;
	char** argv;
	char* pname;
	pid_t pid;
	int number_of_remove_args = 0;
	int job_index = 0;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		for(int x = 0; x < proc_no; x++)
		{
			if(bg_jobs[x].pid == pid)
			{
				flag = true;
				argv = bg_jobs[x].agrv;
				number_of_remove_args = bg_jobs[x].number_of_args;

				for(int y = x; y < proc_no - 1; y++)
				{
					bg_jobs[y].agrv = bg_jobs[1 + y].agrv;
					bg_jobs[y].pid = bg_jobs[1 + y].pid;
					bg_jobs[y].number_of_args = bg_jobs[1 + y].number_of_args;
				}
				proc_no--;
				int cur_rem = 0;
				break;
			}
		}

		if(!flag)
			continue;
		// for (int x = 0; x < 10; x++)
		//     printf("%s", argv[x]);
		if(!WEXITSTATUS(status) && WIFEXITED(status))
			fprintf(stderr, "\n%s with PID %d exited normally.\n", argv[0], pid);
		else
			fprintf(stderr, "\n%s with PID %d exited abnormally.\n", argv[0], pid);

		restart_loop = true;
		for(int y = 0; y < number_of_remove_args; y++)
		{
			if(argv[y] != NULL)
				free(argv[y]);
		}
		if(argv != NULL)
			free(argv);
		fflush(stdin);
		fflush(stdout);
	}
	// putc( '\n', stdin);
	// getc(stdin);
	return;
}

int main()
{
	proc_no = 0;
	signal(SIGINT, sigintHandler);
	signal(SIGCHLD, finish_proc);

	bg_jobs = (struct bg_process*)calloc(100 ,  sizeof(struct bg_process));
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
		// setbuf(stdout, NULL);
		char* input = calloc(4096, sizeof(char));
		char* return_value = fgets(input, 4096, stdin);
		
		if(return_value == NULL || feof(stdin))
		{

			printf("\nCtrl+C (SIGINT) received. Cleaning up and exiting...\n");
			add_pastevent("exit", home_dir);
			exit(0);
		}
		input = strtok(input, "\n");
		if(input == NULL)
			continue;
		char* each_command;
		char *commandpointer, *tokenpointer;

		add_pastevent(input, home_dir);
		int flag = 0;
		each_command = strtok_r(input, ";", &commandpointer);
		if(each_command == NULL)
			continue;

		while(each_command != NULL)
		{
			char* token = strtok_r(each_command, " \t\n", &tokenpointer);
			if(token == NULL)
			{
				each_command = strtok_r(NULL, ";", &commandpointer);
				continue;
			}
			char* command = (char*)calloc(strlen(token) + 10, sizeof(char));
			strcpy(command, token);
			char** args = (char**)calloc(100, sizeof(char*));
			int i = 0;
			token = strtok_r(NULL, " \t\n", &tokenpointer);
			while(token != NULL)

			{
				args[i] = (char*)calloc(strlen(token) + 10, sizeof(char));
				args[i] = token;
				i++;
				token = strtok_r(NULL, " \t\n", &tokenpointer);
			}
			args[i] = NULL;
			int flag = 0;
			//checking for background processes
			if(i == 0 && command[strlen(command) - 1] == '&')
			{
				flag = 1;
				command[strlen(command) - 1] = '\0';
			}
			else if(i != 0 && args[i - 1][strlen(args[i - 1]) - 1] == '&')
			{
				flag = 1;
				args[i - 1][strlen(args[i - 1]) - 1] = '\0';
				if(args[i - 1][0] == '\0')
				{
					args[i - 1] = NULL;
					i--;
				}
			}

			if(flag)
			{
				processes(command, i, args, home_dir, true);
				each_command = strtok_r(NULL, ";", &commandpointer);
				free(args);
				free(command);
				free(token);

				continue;
			}

			if(!strcmp(command, "pastevents"))
			{
				// handle_pastevents(
				// 	input, command, args, i, pwd, home_dir, previous_directory, temp_directory);
				int zz = 0;
			}
			else
			{
				execute(input, command, args, i, pwd, home_dir, previous_directory, temp_directory);
			}
			if(commandpointer != NULL)
				each_command = strtok_r(NULL, ";", &commandpointer);
			else
			{
				each_command = NULL;
			}
			free(args);
			free(command);
			free(token);
		}
		getcwd(pwd, sizeof(pwd));
		free(input);
	}
	free(previous_directory);

	return 0;
}

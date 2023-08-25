#include "pastevents.h"
#include "headers.h"

void print_pastevents(char* home_dir)
{
	char* path = calloc(1000, sizeof(char));
	strcpy(path, home_dir);
	strcat(path, "/pastevents_.txt");
	FILE* read_file = fopen(path, "r");
	if(read_file == NULL)
	{
		perror("Error reading pastevents_.txt");
		return;
	}
	char line[1000];
	int command_count = 0;
	char last_command[10000];
	char history_commands[MAX_HISTORY + 5][1000];
	if(read_file != NULL)
		while(fgets(line, sizeof(line), read_file))
		{
			strcpy(history_commands[command_count], line);
			command_count++;
		}
	if(read_file != NULL)
		fclose(read_file);
	//print in reverse order
    for(int x = command_count - 1; x >= 0; x--)
    {
        printf("%s", history_commands[x]);
    }
	return;
}

void pastevent_purge(char* home_dir)
{
	char* path = calloc(1000, sizeof(char));
	strcpy(path, home_dir);
	strcat(path, "/pastevents_.txt");
	FILE* clear_file = fopen(path, "w");
	if(clear_file == NULL)
	{
		perror("Error clearing pastevents_.txt");
		return;
	}
	// fputs("", clear_file);
	fclose(clear_file);
}

char* give_command(int command_number, char* home_dir)
{
    char* command_to_be_returned = calloc(1000, sizeof(char));
	char* path = calloc(1000, sizeof(char));
	strcpy(path, home_dir);
	strcat(path, "/pastevents_.txt");
	FILE* read_file = fopen(path, "r");
	if(read_file == NULL)
	{
		perror("Error reading pastevents_.txt");
		return NULL;
	}
	char line[1000];
	int command_count = 0;
	char last_command[10000];
	char history_commands[MAX_HISTORY + 5][1000];
	if(read_file != NULL)
		while(fgets(line, sizeof(line), read_file))
		{
			strcpy(history_commands[command_count], line);
			command_count++;
		}
	if(read_file != NULL)
		fclose(read_file);
	if(command_number > command_count)
	{
		printf("No such command exists\n");
		return NULL;
	}
	strcpy(command_to_be_returned, history_commands[command_count - command_number]);
	// puts(command_to_be_returned);
    command_to_be_returned = strtok(command_to_be_returned, "\n");
	return command_to_be_returned;
}

void add_pastevent(char* command, char* home_dir)
{

	char* path = calloc(1000, sizeof(char));
	strcpy(path, home_dir);
	strcat(path, "/pastevents_.txt");
	FILE* read_file = fopen(path, "r");
	if(read_file == NULL)
	{
		perror("Error reading pastevents_.txt");
		return;
	}
	char line[1000];
	int command_count = 0;
	char last_command[10000];
	char history_commands[MAX_HISTORY + 5][1000];
	if(read_file != NULL)
		while(fgets(line, sizeof(line), read_file))
		{
            if (line[0] == '\n')
                continue;
			strcpy(history_commands[command_count], line);
			command_count++;
		}
	if(read_file != NULL)
		fclose(read_file);
	if(strcmp(history_commands[command_count - 1], command) == 0)
	{
		return;
	}

	FILE* clear_file = fopen(path, "w");
	fputs("", clear_file);
	fclose(clear_file);

	FILE* append_file = fopen(path, "a");
	int starting_val = 0;
	if(command_count >= MAX_HISTORY)
		starting_val = 1;
	for(int x = starting_val; x < command_count; x++)
	{

		fputs(history_commands[x], append_file);
	}
	if(command_count != 0)
		fputs("\n", append_file);
	fputs(command, append_file);
	fclose(append_file);
}
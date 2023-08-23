#include "headers.h"
#include "pastevents.h"


void print_pastevents(void)
{
    FILE* read_file = fopen("./pastevents_.txt", "r");
    if(read_file == NULL)
    {
        perror("Error reading pastevents_.txt");
        return;
    }
    char line[1000];
    int command_count = 0;
    char last_command[10000];
    char history_commands[MAX_HISTORY+5][1000];
    if(read_file != NULL)
        while(fgets(line, sizeof(line), read_file))
        {
            strcpy(history_commands[command_count], line);
            command_count++;
        }
    if(read_file != NULL)
        fclose(read_file);
    for(int x = 0; x < command_count; x++)
    {
        printf("%d. %s", x+1, history_commands[x]);
    }
    return;

}

void pastevent_purge(void)
{
    FILE *clear_file = fopen("./pastevents_.txt", "w");
    if (clear_file == NULL)
    {
        perror("Error clearing pastevents_.txt");
        return;
    }
    fputs("", clear_file);
    fclose(clear_file);
}

char*  give_command(int command_number )
{
    char* command_to_be_returned = calloc(1000, sizeof(char));
    FILE *read_file = fopen("./pastevents_.txt", "r");
    if (read_file == NULL)
    {
        perror("Error reading pastevents_.txt");
        return NULL;
    }
    char line[1000];
    int command_count = 0;
    char last_command[10000];
    char history_commands[MAX_HISTORY+5][1000];
    if (read_file != NULL)
        while (fgets(line, sizeof(line), read_file))
        {
            strcpy(history_commands[command_count], line);
            command_count++;
        }
    if (read_file != NULL)
        fclose(read_file);
    if (command_number > command_count)
    {
        printf("No such command exists\n");
        return NULL;
    }
    strcpy(command_to_be_returned, history_commands[command_count - command_number]);
    puts(command_to_be_returned);
    return command_to_be_returned;
    
}


void add_pastevent(char* command )
{

    
    FILE *read_file = fopen("./pastevents_.txt", "r");\
    if (read_file == NULL)
    {
        perror("Error reading pastevents_.txt");
        return;
    }
    char line[1000];
    int command_count = 0;
    char last_command[10000];
    char history_commands[MAX_HISTORY+5][1000];
    if (read_file != NULL)
        while (fgets(line, sizeof(line), read_file))
        {
            strcpy(history_commands[command_count], line);
            command_count++;
        }
    if (read_file != NULL)
        fclose(read_file);
    if (strcmp(history_commands[command_count - 1], command) == 0)
    {
        return;
    }

    FILE *clear_file = fopen("./pastevents_.txt", "w");
    fputs("", clear_file);
    fclose(clear_file);

    FILE *append_file = fopen("./pastevents_.txt", "a");
    int starting_val = 0;
    if (command_count == MAX_HISTORY)
        starting_val = 1;
    for (int x = starting_val; x < command_count; x++)
    {

        fputs(history_commands[x], append_file);
    }
    if (command_count != 0)
        fputs("\n", append_file);
    fputs(command, append_file);
    fclose(append_file);

    
}
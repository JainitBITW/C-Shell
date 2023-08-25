#ifndef __PASTEVENT_H
#define __PASTEVENT_H

// void pastevents(int number_of_commands);
void add_pastevent(char *command , char *home_dir);
char *give_command(int command_number, char *home_dir);
void pastevent_purge(char *home_dir);
void print_pastevents( char *home_dir);
#endif

#ifndef __PASTEVENT_H
#define __PASTEVENT_H

// void pastevents(int number_of_commands);
void add_pastevent(char *command);
char *give_command(int command_number);
void pastevent_purge(void);
void print_pastevents(void);
#endif

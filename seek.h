#include "headers.h"
#ifndef SEEK_H
#define SEEK_H

void seek( char* command ,int i ,char* target, bool flag_f , bool flag_e , bool flag_d , char* home_dir , char* tofind );


void travel_recursive(char* tofind , char* target, char* current_path , char ** results , bool flag_f , bool flag_e , bool flag_d, int* result_count);

#endif
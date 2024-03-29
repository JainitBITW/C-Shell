#include "peek.h"
#include "headers.h"

void print_file_data(char* file_name, char* file_path)
{

	struct stat file_stats;
	int stat_result = stat(file_path, &file_stats);
	if(stat_result == -1)
	{
		char err_buf[1100];
		sprintf(err_buf, "Error reading %s", file_name);
		perror(err_buf);
		return;
	}
	char* file_type = calloc(1000, sizeof(char));

	if(S_ISDIR(file_stats.st_mode))
		file_type[0] = 'd';
	else
		file_type[0] = '-';
	if(file_stats.st_mode & S_IRUSR)
		file_type[1] = 'r';
	else
		file_type[1] = '-';

	if(file_stats.st_mode & S_IWUSR)
		file_type[2] = 'w';
	else
		file_type[2] = '-';
	if(file_stats.st_mode & S_IXUSR)
		file_type[3] = 'x';
	else
		file_type[3] = '-';
	if(file_stats.st_mode & S_IRGRP)
		file_type[4] = 'r';
	else
		file_type[4] = '-';
	if(file_stats.st_mode & S_IWGRP)
		file_type[5] = 'w';
	else
		file_type[5] = '-';
	if(file_stats.st_mode & S_IXGRP)
		file_type[6] = 'x';
	else
		file_type[6] = '-';
	if(file_stats.st_mode & S_IROTH)
		file_type[7] = 'r';
	else
		file_type[7] = '-';
	if(file_stats.st_mode & S_IWOTH)
		file_type[8] = 'w';
	else
		file_type[8] = '-';
	if(file_stats.st_mode & S_IXOTH)
		file_type[9] = 'x';
	else
		file_type[9] = '-';

	printf(ANSI_COLOR_BLUE "%s " ANSI_COLOR_RESET, file_type);

	free(file_type);
	struct passwd* pw = getpwuid(file_stats.st_uid);
	if(pw == NULL)
	{
		char err_buf[1100];
		sprintf(err_buf,
				ANSI_COLOR_RED "\rError getting owner name of %s\t\t\t" ANSI_COLOR_RESET,
				file_name);
		perror(err_buf);
		return;
	}
	printf("  %10s", pw->pw_name);
	struct group* grp = getgrgid(file_stats.st_gid);
	if(grp == NULL)
	{
		char err_buf[1100];
		sprintf(err_buf,
				ANSI_COLOR_RED "\rError getting owner name of %s\t\t\t" ANSI_COLOR_RESET,
				file_name);
		perror(err_buf);
		return;
	}
	printf("  %10s", grp->gr_name);
	long sizeKB = (file_stats.st_size + 1023) / 1024;
	printf(" %10ldKB", sizeKB);
	struct tm* time = localtime(&file_stats.st_mtim.tv_sec);
	char month_array[12][20] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	printf(ANSI_COLOR_MAGENTA " %s", month_array[time->tm_mon]);
	printf(" %2d", time->tm_mday);
	printf(" %d" ANSI_COLOR_RESET, 1900 + time->tm_year);
	printf(ANSI_COLOR_CYAN "  %s" ANSI_COLOR_RESET, file_name);
	printf("\n");
}

void peek(char* home_dir, int a_flag, int l_flag, char* argument)
{
	if(strlen(argument) == 0)
		argument[0] = '.';
	if(argument[0] == '~')
	{
		char* new_argument = calloc(1000, sizeof(char));
		strcpy(new_argument, home_dir);
		strcat(new_argument, argument + 1);
		argument = new_argument;
	}

	//Testing if a file or directory
	struct stat stats;
	// printf("%s\n", argument);
	int stat_return = stat(argument, &stats);
	if(stat_return != 0)
	{
		perror("Cannot open directory or file");
		return;
	}

	//Code for a directory
	if(S_ISDIR(stats.st_mode))
	{
		struct dirent* next_file;
		DIR* dire = opendir(argument);

		if(dire == NULL)
		{
			perror("Couldnt open directory");
		}

		char* file_name = calloc(1000, sizeof(char));
		if(!dire)
		{
			perror("Opening Directory");
			return;
		}
		next_file = readdir(dire);
		while((next_file != NULL))
		{
			strcpy(file_name, next_file->d_name);
			if(!a_flag && file_name[0] == '.')
			{
				next_file = readdir(dire);
				continue;
			}

			if(!l_flag)
			{
				printf("%s\n", next_file->d_name);
				next_file = readdir(dire);
				continue;
			}
			char file_path[10000];
			strcpy(file_path, argument);
			strcat(file_path, "/");
			strcat(file_path, file_name);
			print_file_data(file_name, file_path);
			next_file = readdir(dire);
		}
		return;
	}

	//Code for a file
	int length = strlen(argument);
	int break_index = 0;
	for(int x = 0; x < length; x++)
		if(argument[x] == '/')
			break_index = x + 1;

	char* file_name = (char*)calloc(1000, sizeof(char));
	strcpy(file_name, argument + break_index);
	if(l_flag)
		print_file_data(file_name, argument);
	else
		printf("%s\n", file_name);
	return;
}

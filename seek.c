#include "seek.h"
#include "headers.h"

void travel_recursive(char* tofind,
					  char* target,
					  char* current_path,
					  char** results,
					  bool flag_f,
					  bool flag_e,
					  bool flag_d,
					  int* result_count)
{

		printf("target is %s\n", target);
		printf("tofind is %s\n", tofind);
	    printf("current path is %s\n", current_path);
	printf("target is %d %d \n", (int)flag_f , (int)flag_d );

	char** curr_folders = calloc(1000, sizeof(char*));
	for(int i = 0; i < 1000; i++)
		curr_folders[i] = calloc(1000, sizeof(char));
	int curr_folder_count = 0;
	struct stat stats;
	int stat_return = stat(target, &stats);
	if(stat_return != 0)
	{
		perror("Cannot open directory or file");
		return;
	}
	char** current_directories = calloc(100, sizeof(char*));
	for(int i = 0; i < 100; i++)
		current_directories[i] = calloc(1000, sizeof(char));

	if(S_ISDIR(stats.st_mode))
	{
		// printf("here");
		struct dirent* next_file;
		DIR* dire = opendir(target);
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
			memset(file_name, '\0', 1000);
			strcpy(file_name, next_file->d_name);
			// printf("file name is %s\n", file_name);
			if(strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0)
			{
				next_file = readdir(dire);
				continue;
			}
			char* temp = calloc(1000, sizeof(char));
			// strcpy(temp, current_path);
			// strcat(temp, "/");
			// strcat(temp, file_name);

			strcpy(curr_folders[curr_folder_count], file_name);
			curr_folder_count++;
			next_file = readdir(dire);
			free(temp);
		}
	}
	int curr_dir_count = 0;
	//saving the folders in current_directory
	for(int i = 0; i < curr_folder_count; i++)
	{
		//check if current folder is a directory
		struct stat stats;
		char* temp = calloc(1000, sizeof(char));
		strcpy(temp, target);
		// strcat(temp, "/");
		// printf("temp is %s\n", current_path);
		// strcat(temp, current_path + 1);
		strcat(temp, "/");
		strcat(temp, curr_folders[i]);

		int stat_return = stat(temp, &stats);
		if(stat_return != 0)
		{
			perror("Cannot open directory or file");
			// puts(temp);
			return;
		}
		if(S_ISDIR(stats.st_mode))
		{
			// printf("temp is %s\n", curr_folders[i]);
			if((!strncmp(curr_folders[i], tofind, strlen(tofind))) && flag_d)
			{
				char* temp_dir_name = calloc(1000, sizeof(char));
				strcpy(temp_dir_name, current_path);
				strcat(temp_dir_name, "/");
				strcat(temp_dir_name, curr_folders[i]);
				strcpy(results[*result_count], temp_dir_name);
				(*result_count)++;
				// printf("%s\n",temp_dir_name );
			}
			strcpy(current_directories[curr_dir_count], curr_folders[i]);
			// printf("current directory is %s\n", current_directories[curr_dir_count]);
			curr_dir_count++;
		}
		else
		{
			// printf("temp is %s\n",  curr_folders[i]);
			if((!strncmp(curr_folders[i], tofind, strlen(tofind))) && flag_f)
			{
				char* temp_dir_name = calloc(1000, sizeof(char));
				strcpy(temp_dir_name, current_path);
				strcat(temp_dir_name, "/");
				strcat(temp_dir_name, curr_folders[i]);
				strcpy(results[*result_count], temp_dir_name);
				// printf("%s\n",temp_dir_name );
				(*result_count)++;
			}
		}
	}
	for(int i = 0; i < curr_dir_count; i++)
	{
		char* temp = calloc(1000, sizeof(char));
		strcpy(temp, current_path);
		strcat(temp, "/");
		strcat(temp, current_directories[i]);
		// printf("temp is %s\n", temp);
		// printf("current path is %s\n", current_path);

		char* temptarget = calloc(1000, sizeof(char));
		strcpy(temptarget, target);
		strcat(temptarget, "/");
		strcat(temptarget, current_directories[i]);

		travel_recursive(tofind, temptarget, temp, results, flag_f, flag_e, flag_d, result_count);
		free(temp);
		free(temptarget);
	}
	for(int i = 0; i < 1000; i++)
		free(curr_folders[i]);
	free(curr_folders);
	for(int i = 0; i < 100; i++)
		free(current_directories[i]);
	free(current_directories);
}

void seek(char* command,
		  int i,
		  char* target,
		  bool flag_f,
		  bool flag_e,
		  bool flag_d,
		  char* home_dir,
		  char* tofind)
{
	if(strlen(target) == 0)
	{
		target[0] = '.';
	}
	else if(target[0] == '~')
	{
		char temp[1000];
		strcpy(temp, home_dir);
		strcat(temp, target + 1);
		strcpy(target, temp);
	}
	if(target[0] == '.')
	{
		char* temp = calloc(1000, sizeof(char));
		getcwd(temp, 1000);
		strcat(temp, target + 1);
		strcpy(target, temp);
		free(temp);
	}
	if(target[strlen(target) - 1] == '/')
	{
		target[strlen(target) - 1] = '\0';
	}
	// printf("target is %d %d \n", (int)flag_e , (int)flag_d );

	struct stat stats;
	int stat_return = stat(target, &stats);
	if(stat_return != 0)
	{
		perror("Cannot open directory or file");
		return;
	}
	char* curent_path = calloc(1000, sizeof(char));
	char** results = calloc(1000, sizeof(char*));
	for(int i = 0; i < 1000; i++)
		results[i] = calloc(1000, sizeof(char));
	strcpy(curent_path, ".");
	int result_count = 0;
	travel_recursive(tofind, target, curent_path, results, flag_f, flag_e, flag_d, &result_count);

	if(flag_e && result_count == 1)
	{
		char* new_path = calloc(1000, sizeof(char));
		strcpy(new_path, target);
		strcat(new_path, results[0] + 1);
		struct stat stats;
		int stat_return = stat(new_path, &stats);
		if(flag_f || stat_return)
		{

			if(access(new_path, R_OK) != 0)
			{
				perror("Missing read permission for directory");
				free(new_path);
				return;
			}
			FILE* file = fopen(new_path, "r");

			if(file == NULL)
			{
				perror("Error opening the file");
				return;
			}

			// Read and print the contents character by character
			int c;
			while((c = fgetc(file)) != EOF)
			{
				putchar(c); // Print the character to the console
			}
			printf("\n");
			// Close the file
			fclose(file);
			free(new_path);

			// Check read permission for the directory
			return;
		}
		if(flag_d || (!stat_return))
		{

			if(access(new_path, X_OK) != 0)
			{
				perror("Missing execute permission for directory");
				free(new_path);
				return;
			}
			int return_value = chdir(new_path);
			if(return_value != 0)
			{
				perror("Error changing directory");
			}

			return;
		}

	
		if(stat_return != 0)
		{
			perror("Cannot open directory or file");
			return;
		}
		if(S_ISDIR(stats.st_mode))
		{
			int return_value = chdir(new_path);
			if(return_value != 0)
			{
				perror("Error changing directory");
			}
			return;
		}
	}
	if(result_count == 0)
	{
		printf("No results found\n");
		return;
	}

	for(int i = 0; i < result_count; i++)
	{
		if(strlen(results[i]) != 0)
			printf("%s\n", results[i]);
		//     else
		//     // { cout<<1
		//         // break;}
		//         int zz =0;
		// }
	}
	free(results);
	free(curent_path);
}
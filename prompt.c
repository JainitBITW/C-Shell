#include "headers.h"

char* process_path(char* pwd, char* home_dir)
{
	char temp_path[10000];
	memset(temp_path, '\0', sizeof(temp_path));
	assert(temp_path != NULL);

	char* processed_path = malloc(10000*sizeof(char));
    memset(processed_path, '\0', sizeof(processed_path));
    assert(processed_path != NULL);

	long long int home_dir_len = strlen(home_dir);
	long long int pwd_len = strlen(pwd);
     strncpy( temp_path, pwd, home_dir_len);


	if(pwd_len < home_dir_len)
	{
		strcpy(processed_path, pwd); 
        return processed_path;

	}
   
    if ( pwd[home_dir_len] != '\0' && pwd[home_dir_len]!= '/')
    {
        strcpy(processed_path, pwd);
        return processed_path;
    }
    // If temp_path matches the home_dir, make the path relative to home_dir
    if (strcmp(temp_path, home_dir) == 0)
    {
       
        strcpy(processed_path, "~");
        strcat(processed_path, pwd+home_dir_len);

        // We can free the memory allocated to processed_path to avoid excess memory usage
        char *return_string = calloc((strlen(processed_path) + 5), sizeof(char));
        strcpy(return_string, processed_path);
        free(processed_path);
        return return_string;
       
    }
   
        strcpy(processed_path, pwd);
        return processed_path;
    
}

void prompt(char* cwd , char* home_dir)
{
	// Getting the username :
	char hostname[10000];
    memset(hostname, '\0', sizeof(hostname));
	gethostname(hostname, sizeof(hostname));
	assert(hostname != NULL);

	// Getting the current working directory :


	assert(cwd != NULL);
	char* path_name = process_path(cwd, home_dir);

	// Getting the username :
	char* username = getlogin();
	assert(username != NULL);

	// relative the cwd
	printf("<%s@%s:%s>", username, hostname, path_name);

	// Setting the home directory ;
}

#include "environmentVar.h"

int main(int argc, char ** argv)
{
	if(!strcmp(argv[1], "echo"))
	{

		//Call echo command passing variable to echo
		echo_var(argv);
		exit(0);
	}
	else
	{
		print_env();
		exit(0);
	}
	return 0;
}

void print_env()
{
	extern char ** environ; 

	int i = 0; 
	while(environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}

}


void echo_var(char ** argv)
{
	pid_t pid;
	int status; 

	//extern char ** environ;
	
	char * varCopy = argv[2];
	//remove $ from beginning of var to find. 
	//ex: $PATH to find PATH variable, search using PATH
	if(varCopy[0] == '$') // should this be strcmp???
		//printf("%s\n", varCopy);
		memmove(varCopy, varCopy + 1, strlen(varCopy));
		argv[2] = varCopy;
	
	//printf("%s\n", "calling execvp");
	pid = fork();
	if(pid == 0) // If the child
	{
		status = execvp(argv[1], argv);

		if(status < 0)
		{
			printf("%s", "Unable to call system command: " );
			printf("%s\n", argv[2]);
			exit(0);
		}
	}
	else
	{
		//In parent process, wait for child to finish.
		if (waitpid(pid, &status, 0) < 0)
			printf("%d %s", pid, "This was the unix error");
		return;
	}

	return;

	/*
	//Print the variable value if it exists, blank if not
	int i = 0; 
	char copy[1000];
	char * token;
	char * environHold;
	while(environ[i])
	{
		environHold = environ[i];
		//Split the environment variable by =
		strcpy(copy, environHold);
		token = strtok(copy, "=");

		if(!strcmp(token, varCopy))
		{
			printf("%s\n", environHold);
			return(1);
		}		
		//If the first value = the input envVar, print, else continue
		i++;
	}

	printf("%s\n", "");
	return(1); */
} 
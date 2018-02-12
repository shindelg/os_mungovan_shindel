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


int echo_var(char ** envVar)
{
	extern char ** environ;
	char * varCopy = envVar[2];
	//remove $ from beginning of var to find. 
	//ex: $PATH to find PATH variable, search using PATH
	if(varCopy[0] == '$')
		memmove(varCopy, varCopy + 1, strlen(varCopy));
	
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
	return(1);
} 
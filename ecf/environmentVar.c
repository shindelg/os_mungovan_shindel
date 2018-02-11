#include "environmentVar.h"

int main(int argc, char **argv)
{
	if(!strcmp(argv[0], "echo"))
	{
		//Call echo command passing variable to echo
		//echo_var(char *argv[2]);
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
	extern char** environ; 

	int i = 0; 
	while(environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}

}


void echo_var(char *envVar)
{
	extern char** environ;
	char *varCopy = envVar;
	//Print the variable value if it exists, blank if not
	//O = False, 1 = True
	int found = 0; 
	int i = 0; 
	char *token; 
	char *environHold;
	while(environ[i])
	{
		environHold = environ[i];
		//Split the environment variable by =
		token = strtok(environHold, "=");
		if(!strcmp(token, envVar))
		{
			found = 1; 
			printf("%s\n", token);
			break;
		}
		//If the first value = the input envVar, print, else continue
		i++;
	}

	if(!found)
	{
		printf("%s\n", "");
	}
} 
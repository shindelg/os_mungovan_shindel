#include "environmentVar.h"
void print_env(char ** argv)
{

	pid_t pid;
	int status; 

	pid = fork();
	if(pid == 0) // If the child
	{
		printf("%s", "argv[0]: ");
		printf("%s\n", argv[0]);

		status = execvp(*argv, argv);

		if(status < 0)
		{
			printf("%s", "Unable to call system command: " );
			printf("%s\n", argv[0]);
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

}

void modify_env_var(char ** argv)
{
	int splitIndex; 
	char * variableName;
	char * variableValue;

	splitIndex = strchr(variableValue,'=');
	variableName = malloc(sizeof(char) * strlen(argv[0]));
	variableValue = malloc(sizeof(char) * (strlen(argv[0]) - splitIndex));

    strcpy(variableName,argv[0]);

	int valueIndex = 0;
	for(char i = splitIndex; variableName[i] != '\0'; ++i)
	{
		variableValue[valueIndex] = variableName[i];
	}

	//split str by = 
	split_str(variableName);

	//If the variable value is only =, then remove the variable
	if(strchr(variableValue,'='))
	{
		unsetenv(variableName);
	}
	//else add the variable into the environment
	else
	{
		setenv(variableName,variableValue,1);
	}

	free(variableName);
	free(variableValue);
	return;

}

void echo_var(char ** argv)
{
	char * variableName;

	char * variableValue;

	//If variable is $PWD, remove $
	if(strchr(argv[1],'$'))
	{
		variableName = malloc(sizeof(char) * (strlen(argv[1] -1)));
		argv[1]++;

	}
	else
	{
		variableName = malloc(sizeof(char) * strlen(argv[1]));
	}

    strcpy(variableName,argv[1]);

    variableValue = getenv(variableName);

    if(variableValue)
    {
    	printf("%s", variableName);
    	printf("%s", "=");
    	printf("%s\n", variableValue);
    }
    else
    {
    	printf("%s", "Variable: ");
    	printf("%s", variableName);
    	printf("%s\n", " not found in environment.");
    }

    return;


	/*
	pid_t pid;
	int status; 

	//extern char ** environ;
	
	char * varCopy;
	//If no environmental variable following echo
	if(argv[1] == NULL)
	{
		printf("%s\n", "Echo requires two arguments");
		return;
	}
	//remove $ from beginning of var to find. 
	//ex: $PATH to find PATH variable, search using PATH
	
	
	if(varCopy[1] == '$' && !(varCopy == NULL)) // should this be strcmp???
		//printf("%s\n", varCopy);
		memmove(varCopy, varCopy + 1, strlen(varCopy));
		argv[1] = varCopy; 
	
	//printf("%s\n", "calling execvp");

	pid = fork();
	if(pid == 0) // If the child
	{
		printf("%s", "argv[0]: ");
		printf("%s\n", argv[0]);

		printf("%s", "argv[1]: ");
		printf("%s\n", argv[1]);

		status = execvp(*argv, argv);

		if(status < 0)
		{
			printf("%s", "Unable to call system command: " );
			printf("%s\n", argv[0]);
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

char * split_str(char * string)
{
	char * retString;

	retString = strtok(string, "=");

	return(retString);


}
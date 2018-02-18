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

	variableName = NULL;
	variableValue = NULL;

	return;

}

void echo_var(char ** argv)
{
	char * variableName = NULL;

	char * variableValue = NULL;

	//If variable is $PWD, remove $
	if(strchr(argv[1],'$'))
	{
		variableName = malloc(strlen(argv[1]) + 1 );
		argv[1]++;

	}
	else
	{
		variableName = malloc(strlen(argv[1]) + 1);
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

    if(variableName != NULL)
    {/*
    	free(variableName);
    	variableName = NULL;*/
    }

    if(variableValue != NULL)
    {/*
    	free(variableValue);
    	variableValue = NULL;*/
    }

    return;
} 

char * split_str(char * string)
{
	char * retString;

	retString = strtok(string, "=");

	return(retString);


}
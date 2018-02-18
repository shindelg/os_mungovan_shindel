#include "pipes.h"


int main()
{
	char * argv[4];

	argv[0] = "ls";
	argv[1] = "|";

	argv[2] = "more";
	argv[3] = NULL;

	// char * argv[2];

	// char * argc[2];

	// argv[0] = "ls";
	// argv[1] = NULL;

	// argc[0] = "more";
	// argc[1] = NULL;

	makePipe(argv);

}

int makePipe(char ** argv)
{
	char * arga[10];

	char * argb[10];

	int index = 0; 
	int argvLoc = 0;
	while(argv[argvLoc] != NULL)
	{
		if(!(strchr(argv[argvLoc], '|')))
		{
			arga[index] = argv[argvLoc];
			argvLoc++;
			index++;
		}
		else
		{
			argvLoc++;
			arga[index] = NULL;
			index = 0;
			break;
		}
	}

	while(argv[argvLoc] != NULL)
	{
		argb[index] = argv[argvLoc];
		argvLoc++;
		index++;
	}

	createPipe(arga, argb);
}

int createPipe(char ** firstProcArguments, char ** secondProcArguments)
{

	//Pids for subprocesses
	pid_t pidFirstProc, pidSecProc; 

	//Status of processes
	int status; 

	//Storage for file descriptors created by pipe
	int fileDescriptors[2];

	// Create a pipe.
   pipe(fileDescriptors);
   // Create our first process.
   pidFirstProc = fork();
   if (pidFirstProc == 0) 
   {
		//attach the write end of pipe to stdout
		dup2(fileDescriptors[1], STDOUT_FILENO);

		//remove read end of the pipe, not used in this process. 
		//first process outputs only to write end
		close(fileDescriptors[0]);

		//Run first process, save status
		status = execvp(firstProcArguments[0], firstProcArguments);
		if(status < 0)
		{
			printf("%s", "Unable to call system command: " );
			printf("%s\n", firstProcArguments[0]);
			exit(0);		
		}
   }
   // Create the second process.
   pidSecProc = fork();
   if (pidSecProc == 0) 
   {
   		//Overwrite stdin with read end of pipe 
		dup2(fileDescriptors[0], STDIN_FILENO);

		//Run first process, save status
		close(fileDescriptors[1]);
		status = execvp(secondProcArguments[0], secondProcArguments);
		if(status < 0)
		{
			printf("%s", "Unable to call system command: " );
			printf("%s\n", secondProcArguments[0]);
			exit(0);		
		}
   }
   //Close pipe ends
   close(fileDescriptors[0]);
   close(fileDescriptors[1]);

   //Wait for two processes to finish
   if (waitpid(pidFirstProc, &status, 0) < 0)
   {
   	fprintf(stderr, "%s\n", "error 1");
   }

   if (waitpid(pidSecProc, &status, 0) < 0)
   {
   	fprintf(stderr, "%s\n", "error 2");
   }
   return 0;
}


int findIndexOfPipe(char ** argv)
{
	int index = 0; 

	int i;
	for(i = 0; argv[i] != NULL; i++)
	{
		if(strchr(argv[i],'|'))
		{
			index = i; 
			break;
		}
	}

	return(index);
}

char ** copySubArray(char ** argv, int indexToStop)
{
	char ** returnArr[indexToStop + 1];

	int i; 
	for(i = 0; i < indexToStop; i++)
	{
		strcpy(returnArr[i],&argv[i]);
	}
	/*
	int i = 0; 
	while(i < indexToStop)
	{
		returnArr[i] = *argv[i];
		i++;
	}
	*/
	i++; 
	returnArr[i] = NULL;
	return(copySubArray);
}

char ** copyArrayUntilNull(char ** argv, int startLocation)
{

	char ** returnArr[10];

	int i;
	int index = 0;
	for(i = startLocation; argv[i] != NULL; i++)
	{
		strcpy(returnArr[index], &argv[i]);
		index++;
	}

	index++; 
	returnArr[index] = NULL;

	/*
	int i = 0; 
	int index = startLocation;
	while(argv[index] != NULL)
	{
		returnArr[i] = *argv[index];
		i++; 
		index++; 
	}

	i++; 

	returnArr[i] = NULL;*/

	return(returnArr);

}
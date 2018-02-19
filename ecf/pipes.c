#include "pipes.h"
/*
Input argv, assumes 1 pipe. 

Creates two argument arrays for each process, then calls for 
a pipe execution

*/
void makePipe(char ** argv)
{
	//Two arrays to store commands. 
	char * arga[10];

	char * argb[10];

	int index = 0; 
	int argvLoc = 0;
	//Store all calls before | 
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

	//Store remaining commands following |
	while(argv[argvLoc] != NULL)
	{
		argb[index] = argv[argvLoc];
		argvLoc++;
		index++;
	}

	//Make a pipe, execute commands. 
	executePipe(arga, argb);

	return;
}

/*

Takes in 2 argument arrays. 

Pipes the output of the first into the second. 
*/
void executePipe(char ** firstProcArguments, char ** secondProcArguments)
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
   		fprintf(stderr, "%s\n", "Error in first process.");
   }
   if (waitpid(pidSecProc, &status, 0) < 0)
   {
   		fprintf(stderr, "%s\n", "Error in second process");
   }

   return;
}


/*

Takes in argv from the command line, returns the index of the pipe
character if it exists, 0 if not found.
*/
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
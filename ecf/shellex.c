/* $begin shellmain */
#include "../include/csapp.h"
#include "shellex.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
	/* Read */
	printf("lsh> ");    // promt the user to write something               
	fgets(cmdline, MAXLINE, stdin); 	// waits for input
	if (feof(stdin)) // checks for end-of-file indicator
	    exit(0);

	/* Evaluate */
	eval(cmdline);  // passes input to eval
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
        if ((pid = fork()) == 0) {   /* Child runs user job */
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
			printf("%d %s", pid, "This was the unix error");
		//unix_error("waitfg: waitpid error");
	}
	else
	    printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
    {
	   exit(0);  
    }
	else if (!strcmp(argv[0], "stat"))
    {
		return 1;
    }
    else if(!strcmp(argv[0], "printenv"))
    {
        printf("%s\n", "print environment variables");
        print_env(argv);
        return(1);
    }
    //Split argv[1] based on =, if it contains it then the user is trying to 
    //modify an environmental variable
    else if (split_str(argv[0]))
    {
        char * copyOfArgv; 
        char * cutArgv; 
        strcpy(copyOfArgv, argv[0]);

        
        cutArgv = split_str(copyOfArgv);
        //If user trying to create an environmental variable. 
        int cutArgLength = strlen(&cutArgv);
        int originalArgLength =  strlen(&argv[0]);

        //split the string by =, compare the lengths
        //if the string is varName=
        if(!(cutArgLength + 1 == originalArgLength + 1))
        {
            set_env_var(argv);
            return(1);
        }
        //If the user is trying to remove an environmental variable. 
        else
        {
            remove_env_var(argv);
            return(1);
        } 
        printf("%s\n", "inside add / remove");
        return(1);
    }
    else if(!strcmp(argv[0], "echo"))
    {
        echo_var(argv);
        return(1);
    }
    else if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
    {
	   return 1;
    }
    else
    {
        return 0; /* Not a builtin command */
    }                     
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */



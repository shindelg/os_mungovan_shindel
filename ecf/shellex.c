/* $begin shellmain */
#include "../include/csapp.h"
#include "stats.h"
#include "stats.c"
#include "man.c"
#include "signal.h"

#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
void signal_handler(int);

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

	signal(SIGINT, INThandler);

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
  
void  signal_handler(int sig)
{
     signal(sig, SIG_IGN);
     printf("OUCH, did you hit Ctrl-C?");
}

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
    		/*
            if (execve(argv[0], argv, environ) < 0) {
                exit(0);
            }
            */
          if (execvp(*argv, argv) < 0) {     // run inheruted linux commands 
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
     /*
     pid_t  pid;
     int    status;
     
     if ((pid = fork()) < 0) {     // fork a child process           
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          // for the child process:         
          if (execvp(*argv, argv) < 0) {     // execute the command  
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  // for the parent:      
          while (wait(&status) != pid)       // wait for completion  
               ;
     }
     */

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
		exit(0);  
	else if (!strcmp(argv[0], "man")){
		print_man();
		return 1;
	}
	else if (!strcmp(argv[0], "stat")){
		parse_stats(argv);
		return 1;
	}
    else if (!strcmp(argv[0], "&")){
		return 1;
	}
    else{
    	return 0;                     /* Not a builtin command */
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


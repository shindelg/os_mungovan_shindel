/* $begin shellmain */
#include "../include/csapp.h"
#include "stats.h"
#include "stats.c"
#include "man.c"
#include "signal.h"

#include "shellex.h"
#define MAXARGS   128
#define MAXJOBS	20

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
int confirmjob(int pid); 
int updatejobs(int pid, int status);
void signal_handler(int);
int jid = 1;

struct job_t {
    pid_t pid;
    int jid;
    int state;
    char cmdline[MAXLINE];
};

struct job_t joblist[MAXJOBS];

int main() 
{
    char cmdline[MAXLINE]; /* Command line */

	signal(SIGINT, signal_handler);
	signal(SIGTSTP, signal_handler);

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

void  signal_handler(int sig)
{
	if(sig == SIGTSTP){
		//kill(pid, SIGTSTP);
		printf("%s\n", "Did you hit ctrl+Z");
	}
	if(sig == SIGINT){
		//kill(pid, SIGINT);
		printf("%s\n", "Did you hit ctrl+C");		
	}
     exit(0);
}

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    int arg_stat;		 /* BG/FG to be passed to the job */
    pid_t pid;           /* Process id */


    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { // runs if its a builting
        if ((pid = fork()) == 0) {   /* Child runs user job */
          if (execvp(*argv, argv) < 0) {     // run inheruted linux commands 
               printf("%s: Command not found.\n", argv[0]);
               exit(0);
          }    	
        }

    	if(bg == 1){
    	    arg_stat = 1;
    	}
   		else{
        	arg_stat = 2;

    	}    

    	commitjob(joblist, pid, jid, arg_stat, cmdline); 

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
	print_stats();
	jid++;

    return;
}

int commitjob(struct job_t *joblist, pid_t pid, int jid, int state, char *cmdline){
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
		if (joblist[i].pid == 0) {
	    	joblist[i].pid = pid;
	    	joblist[i].jid = jid;
	    	joblist[i].state = state;
	    	strcpy(joblist[i].cmdline, cmdline);
	    	return 1;
		}
    }
    return 0;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char ** argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
		exit(0);  
	else if (!strcmp(argv[0], "jobs")){
		int i = 0;
		for (i = 0; i < MAXJOBS; i++){
			if(joblist[i].pid != 0){
				printf("%5d", (int)joblist[i].pid);
			}
			if(joblist[i].jid != 0){
				printf("%s %d", "	", joblist[i].jid);
			}
			if(joblist[i].state == 1){
				printf("%s\n", ":	Foreground");
			}
			else if(joblist[i].state == 2){
				printf("%s\n", ":	Background");
			}
			else if(joblist[i].state == 3){
				printf("%s\n", ":	Stopped");
			}
		}
		return 1;
	}
	else if (!strcmp(argv[0], "bg") && argv[1] != NULL){
		int bg_pid;
		bg_pid =  atoi(argv[1]);
		if(confirmjob(bg_pid) == 1){
			printf("%s\n", "command was sent to background.");
			kill(bg_pid, SIGCONT);
			updatejobs(bg_pid, 2);
		}
		else{
			printf("%s\n", "The requested job does not exist.");
		}		
		return 1;
	}
	else if (!strcmp(argv[0], "fg")){
		int fg_pid;
		fg_pid = atoi(argv[1]);
		if(confirmjob(fg_pid) == 1){
			printf("%s\n", "command was sent to foreground.");
			kill(fg_pid, SIGCONT);
			updatejobs(fg_pid, 1);
		}
		else{
			printf("%s\n", "The requested job does not exist.");
		}
		return 1;
	}		
	else if (!strcmp(argv[0], "stat")){
		enable_stats(argv);
		return 1;
	}
    else if(!strcmp(argv[0], "printenv"))
    {
        print_env(argv);
        return(1);
    } 
    else if(strchr(argv[0],'='))
    {   
        modify_env_var(argv);
        return(1);
    }
    else if(!strcmp(argv[0], "echo"))
    {
        echo_var(argv);
        return(1);
    }
    else{
    	return 0;
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

int confirmjob(int pid){
	int i;
	for(int i = 0; i < MAXJOBS; i++){
		if(joblist[i].pid == pid){
			return 1; // what do I want to return here?
		}
		else{
			return 0;
		}
	}
	return 0;
}

int updatejobs(int pid, int status){
	for(int i = 0; i < MAXJOBS; i++){
		if(joblist[1].pid == pid){
			joblist[i].state = status;
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}

void killjobs(int pid, int sig){
	int i;
	for(int i = 0; i < MAXJOBS, i++){
		
	}
}












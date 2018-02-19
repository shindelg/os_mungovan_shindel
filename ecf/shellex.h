#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "environmentVar.h"
#include "environmentVar.c"
#include "pipes.h"
#include "pipes.c"
#include "fork.h"
#include "../include/csapp.h"

#define MAXLINE 8192 /*max text line length */ 
#define MAXBUF 8192 /*max i/o buffer size */ 
extern char **environ; /* Defined by libc */

//Process control wrappers

//Error_handling functions
void unix_error(char *msg);

int builtin_command(char **argv);

int parseline(char *buf, char **argv);
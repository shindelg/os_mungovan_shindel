/* $begin shellmain */
/*#include "csapp.h" */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "main.h"

#define MAXARGS   128

int main() 
{
    //char cmdline[MAXLINE]; /* Command line */

    while (1) {
	/* Read */
	printf("> ");                   
	//Fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin))
	    exit(0);
	}
}
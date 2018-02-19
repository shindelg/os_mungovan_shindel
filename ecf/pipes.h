#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fork.h"

//Open a pipe

void executePipe(char ** firstProcArguments, char ** secondProcArguments);

void makePipe(char ** argv);

int findIndexOfPipe(char ** argv); 
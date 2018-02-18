#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fork.h"

//Open a pipe

int main();

int createPipe(char ** firstProcArguments, char ** secondProcArguments);

int makePipe(char ** argv);

int findIndexOfPipe(char ** argv); 

char ** copySubArray(char ** argv, int indexToStop);

char ** copyArrayUntilNull(char ** argv, int startLocation);

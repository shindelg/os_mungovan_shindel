#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fork.h"

//Prints environment variables 
void print_env();

void echo_var(char **argv);

int main(int argc, char **argv);
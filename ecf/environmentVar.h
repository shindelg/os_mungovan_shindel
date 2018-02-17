#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fork.h"

//Prints environment variables 
void print_env(char ** argv);

void echo_var(char ** argv);

void modify_env_var(char ** argv);

void remove_env_var(char ** argv);

char * split_str(char * string);

//int main(int argc, char **argv);
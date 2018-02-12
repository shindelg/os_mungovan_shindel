#include <stdio.h>
#include <stdlib.h> // For exit()
 
extern void print_man();

void print_man(){
   
    FILE *fptr;
 
 	char c;
    // Open file
    fptr = fopen("./man/man.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
 
    // Read contents from file
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
 
    fclose(fptr);
}
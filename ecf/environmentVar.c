#include "environmentVar.h"

int main()
{
	print_env();
	exit(0);
}

void print_env()
{
	extern char** environ; 

	int i = 0; 
	while(environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}

}
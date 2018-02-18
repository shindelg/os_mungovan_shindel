#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
  pid_t pid[2];
  char *argv2[2][3];
  argv2[0][0] = argv2[1][0] = "/home/ordillej/examples/giveme";
  argv2[0][1] = "pizza";
  argv2[1][1] = "cookies";
  argv2[0][2] = argv2[1][2] = NULL;

  for (int i = 0; i<2; i++) {
    pid[i] = fork();
    if (pid < 0) {
      fprintf(stderr,"%s fork error creating child %d: %s\n", argv[0], i, strerror(errno));
      exit(1);
    }
    if (pid[i] == 0) {  /* Child */
      execv(argv2[i][0], argv2[i]);
    }
  }
  
  /* Parent */
  printf("Parent completing successfully leaving children %d and %d.\n", pid[0], pid[1]);
  exit(0);
}


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    // fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child: redirect standard output to a file
    close(STDOUT_FILENO);
    int fd = open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    fprintf(stderr, "fd: %d\n", fd);

    // now exec "wc"...
    char *myargs[3];
    myargs[0] = strdup("wc");     // program: wc
    myargs[1] = strdup(argv[0]);  // arg: file to count
    myargs[2] = NULL;             // mark end of array
    execvp(myargs[0], myargs);    // runs word count
  } else {
    // parent goes down this path (main)
    int rc_wait = wait(NULL);
  }
  return 0;
}

// https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("hello (pid:%d)\n", (int)getpid());
  int fork_rc = fork();
  if (fork_rc < 0) {  // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (fork_rc == 0) {  // child (new process)
    printf("child (pid:%d)\n", (int)getpid());
  } else {  // parent goes down this path
    int status;
    pid_t rc_wait = waitpid(fork_rc, &status, 0);
    printf("child %d terminated normally with exit code %d\n", rc_wait,
           WEXITSTATUS(status));
    if (rc_wait == -1) {
      perror("waitpid failed");
      exit(1);
    }

    // int rc_wait = wait(NULL);
    printf("parent of %d (rc_wait:%d) (pid:%d)\n", fork_rc, rc_wait,
           (int)getpid());
  }
  return 0;
}

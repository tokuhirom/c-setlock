#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage() {
  printf("setlock: usage: setlock [ -nNxX ] file program [ arg ... ]\n");
  exit(100);
}

int main(int argc, char *const *argv, const char *const *envp)
{
  int opt;
  int fd;
  const char *file;

  int flagndelay = 0;
  int flagx = 0;

  while ((opt = getopt(argc,argv,"nNxX")) != -1) {
    switch(opt) {
      case 'n': flagndelay = 1; break;
      case 'N': flagndelay = 0; break;
      case 'x': flagx = 1; break;
      case 'X': flagx = 0; break;
      default: usage();
    }
  }

  argv += optind;
  if (!*argv) usage();
  file = *argv++;
  if (!*argv) usage();

  fd = open(file, O_WRONLY | O_NDELAY | O_APPEND | O_CREAT, 0600);
  if (fd == -1) {
    if (flagx) _exit(0);
    fprintf(stderr, "setlock: fatal: unable to open '%s'\n",file);
    exit(111);
  }

  if (flock(fd, flagndelay ? LOCK_EX | LOCK_NB : LOCK_EX) == -1) {
    if (flagx) _exit(0);
    fprintf(stderr, "setlock: fatal: unable to lock '%s'\n", file);
    exit(111);
  }

  execvpe(argv[0], argv, envp);
  perror("setlock: fatal: unable to run command");
  exit(111);
}

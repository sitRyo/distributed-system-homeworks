#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void parent(int[2]);
void child(int[2], const char*);

// 0 -> read
// 1 -> write

// blocking
void parent(int fildes[2]) { 
  char c;
  int cnt = 0;

  close(fildes[1]);
  while (read(fildes[0], &c, 1) > 0) {
    write(1, &c, 1);
  }
  close(fildes[0]);
}

void child(int fildes[2], const char * str) {
  char c;
  ssize_t t;
  close(fildes[0]);
  while (*str) {
    c = *str++;
    if (islower(c)) c = toupper(c);
    write(fildes[1], &c, 1);
  }
  close(fildes[1]);
}

int main(void) {
  int fildes[2];
  pid_t pid;
  const char * str = "abcde";

  if (pipe(fildes) == -1) {
    perror("pipe");
    exit(1);
  }

  if ((pid = fork()) == 0) {
    child(fildes, str);
  } else if (pid > 0) {
    parent(fildes);
  } else {
    perror("fork");
    exit(1);
  }
}
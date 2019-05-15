#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void parent(int[2], int[2], const char*);
void child(int[2], int[2]);

// 0 -> read
// 1 -> write

// blocking
void parent(int fildes[2], int fildes2[2], const char * str) { 
  char c;
  int cnt = 0;
  int sz = strlen(str);

  close(fildes[0]);
  close(fildes2[1]);
  while (*str) {
    c = *str++;
    write(fildes[1], &c, 1);
  }

  close(fildes[1]);

  while (read(fildes2[0], &c, 1) > 0) {
    write(1, &c, 1);
  }

  close(fildes2[1]);
}

void child(int fildes[2], int fildes2[2]) {
  char c;
  close(fildes[1]);
  close(fildes2[0]);

  while (read(fildes[0], &c, 1) > 0) {
    if (islower(c)) c = toupper(c);
    write(fildes2[1], &c, 1);
  }

  close(fildes[0]);
  close(fildes2[1]);
}

int main(void) {
  int fildes[2], fildes2[2];
  pid_t pid;
  const char * str = "abcde";

  if (pipe(fildes) == -1) {
    perror("pipe");
    exit(1);
  }

  if (pipe(fildes2) == -1) {
    perror("pipe");
    exit(1);
  }

  if ((pid = fork()) == 0) {
    child(fildes, fildes2);
  } else if (pid > 0) {
    parent(fildes, fildes2, str);
  } else {
    perror("fork");
    exit(1);
  }
}
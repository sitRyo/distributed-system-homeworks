#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void capitalize(void*);

void capitalize(void *str) {
  size_t len = strlen((char*)str);
  size_t i;
  for (i = 0; i < len; ++i) {
    char t = *(char*)(str + i);
    if (islower(t)) {
      t = toupper(t);
      memset(str+i, t, 1);
    }
  }
}

int main() {
  pthread_t thread;
  int iret;

  char str[] = "aBcde";

  iret = pthread_create(&thread, NULL, (void* (*)(void *))capitalize, (void*)str);
  if (iret) {
    perror("thread");
    exit(1);
  }

  pthread_join(thread, NULL);

  printf("%s\n", str);
}

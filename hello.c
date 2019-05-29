#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {
  printf("Hello 1\n");
  sleep(2);
  printf("Hello 2\n");
  sleep(1);
  return 0;
}
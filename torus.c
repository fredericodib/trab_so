#define PROCESS_TABLE 0x199542

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "torus.h"

Process *process_table;

void torus() {
  printf("utilizando torus\n\n");
  recupera_memoria_compartilhada(process_table);
  /* codigo */
}

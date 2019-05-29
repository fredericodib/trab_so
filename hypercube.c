#define PROCESS_TABLE 0x199542

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "hypercube.h"

Process *process_table;

void recupera_memoria_compartilhada_hypercube() {
  int process_table_id;
  process_table_id = shmget( PROCESS_TABLE, 16 * sizeof(Process), 0700);
  if (process_table_id < 0) {
    printf("error ao criar memoria compartilhada\n");
    exit(1);
  }
  process_table = (Process *) shmat(process_table_id, 0, 0777);
}

void hypercube() {
  printf("utilizando hypercube\n\n");
  recupera_memoria_compartilhada_hypercube();
  /* codigo */
}


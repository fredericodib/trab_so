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
  process_table[0].vizinho[0] = 1;
  process_table[0].vizinho[1] = 2;
  process_table[0].vizinho[2] = 4;
  process_table[0].vizinho[3] = 8;

  process_table[1].vizinho[1] = 0;
  process_table[1].vizinho[2] = 9;
  process_table[1].vizinho[0] = 3;
  process_table[1].vizinho[3] = 5;

  process_table[2].vizinho[0] = 0;
  process_table[2].vizinho[1] = 6;
  process_table[2].vizinho[2] = 10;
  process_table[2].vizinho[3] = 3;

  process_table[3].vizinho[0] = 1;
  process_table[3].vizinho[1] = 2;
  process_table[3].vizinho[2] = 7;
  process_table[3].vizinho[3] = 11;

  process_table[4].vizinho[0] = 0;
  process_table[4].vizinho[1] = 5;
  process_table[4].vizinho[2] = 6;
  process_table[4].vizinho[3] = 12;

  process_table[5].vizinho[0] = 1;
  process_table[5].vizinho[1] = 4;
  process_table[5].vizinho[2] = 13;
  process_table[5].vizinho[3] = 7;

  process_table[6].vizinho[0] = 2;
  process_table[6].vizinho[1] = 14;
  process_table[6].vizinho[2] = 7;
  process_table[6].vizinho[3] = 4;

  process_table[7].vizinho[0] = 3;
  process_table[7].vizinho[1] = 5;
  process_table[7].vizinho[2] = 6;
  process_table[7].vizinho[3] = 15;

  process_table[8].vizinho[0] = 0;
  process_table[8].vizinho[1] = 12;
  process_table[8].vizinho[2] = 9;
  process_table[8].vizinho[3] = 10;

  process_table[9].vizinho[0] = 1;
  process_table[9].vizinho[1] = 8;
  process_table[9].vizinho[2] = 13;
  process_table[9].vizinho[3] = 11;

  process_table[10].vizinho[0] = 2;
  process_table[10].vizinho[1] = 8;
  process_table[10].vizinho[2] = 11;
  process_table[10].vizinho[3] = 14;

  process_table[11].vizinho[0] = 3;
  process_table[11].vizinho[1] = 9;
  process_table[11].vizinho[2] = 10;
  process_table[11].vizinho[3] = 15;

  process_table[12].vizinho[0] = 4;
  process_table[12].vizinho[1] = 8;
  process_table[12].vizinho[2] = 13;
  process_table[12].vizinho[3] = 14;

  process_table[13].vizinho[0] = 5;
  process_table[13].vizinho[1] = 9;
  process_table[13].vizinho[2] = 12;
  process_table[13].vizinho[3] = 15;

  process_table[14].vizinho[0] = 6;
  process_table[14].vizinho[1] = 10;
  process_table[14].vizinho[2] = 12;
  process_table[14].vizinho[3] = 15;

  process_table[15].vizinho[0] = 7;
  process_table[15].vizinho[1] = 11;
  process_table[15].vizinho[2] = 13;
  process_table[15].vizinho[3] = 14;
}


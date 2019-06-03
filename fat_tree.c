/*
Alunos:
- Frederico Pinheiro Dib - 15/0125925
- Renan Rocha Reboredo - 130/131865
*/
#define PROCESS_TABLE 0x199542

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "fat_tree.h"

Process *process_table;

void recupera_memoria_compartilhada_fat_tree() {
  int process_table_id;
  process_table_id = shmget( PROCESS_TABLE, 16 * sizeof(Process), 0700);
  if (process_table_id < 0) {
    printf("error ao criar memoria compartilhada\n");
    exit(1);
  }
  process_table = (Process *) shmat(process_table_id, 0, 0777);
}

void fat_tree() {
  printf("utilizando fat_tree\n\n");
  recupera_memoria_compartilhada_fat_tree();
  /* codigo */
  process_table[0].vizinho[0] = 1;
  process_table[0].vizinho[1] = 2;

  process_table[1].vizinho[0] = 3;
  process_table[1].vizinho[1] = 4;

  process_table[2].vizinho[0] = 5;
  process_table[2].vizinho[1] = 6;

  process_table[3].vizinho[0] = 7;
  process_table[3].vizinho[1] = 8;

  process_table[4].vizinho[0] = 9;
  process_table[4].vizinho[1] = 10;

  process_table[5].vizinho[0] = 11;
  process_table[5].vizinho[1] = 12;

  process_table[6].vizinho[0] = 13;
  process_table[6].vizinho[1] = 14;

  process_table[7].vizinho[0] = 15;
}


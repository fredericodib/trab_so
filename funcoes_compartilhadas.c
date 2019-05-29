#define PROCESS_TABLE 0x199542
#define JOB_COUNTER 0x194542
#define CURRENT_JOB 0x259542
#define PROCESS_TABLE_SEM 0x7392871
#define PROCESS_FREEE_SEM 0x7492871
#define JOB_COUNTER_SEM 0x7495871

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "funcoes_compartilhadas.h"

/* recupera process_table compartilhada */
void recupera_memoria_compartilhada(Process *process_table) {
  int process_table_id;
  process_table_id = shmget( PROCESS_TABLE, 16 * sizeof(Process), 0700);
  if (process_table_id < 0) {
    printf("error ao criar memoria compartilhada\n");
    exit(1);
  }
  process_table = (Process *) shmat(process_table_id, 0, 0777);
}

/* verifica se todos os genrentes de execucao estao livres */
int check_todos_os_processos_livres(Process *process_table) {
  int i;
  for (i=0;i<16;i++) {
    if (process_table[i].status == 0) {
      return -1;
    }
  }
  return 1;
}
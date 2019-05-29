#define PROCESS_TABLE 0x199542
#define PROCESS_TABLE_SEM 0x7392871

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "structs.h"
#include "funcoes_compartilhadas.c"
#include "semaphore.h"

Process *process_table;
int ge_id; /* id do gerente */

int main(int argc, char const *argv[]) {
  if (argc !=2 ) { /* verifica numero de argumentos */
    printf("Quantidade de argumentos inválida\n");
    exit(1);
  }

  ge_id = (int)argv[1][0]; /* pega id */

  recupera_memoria_compartilhada(process_table);

  return 0;
}
#define QUEUE_KEY_AT 0x1332624
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
#include "semaphore.h"

Process *process_table;
int job_counter = 1;
Message msg_received;

/* cria fila executa_postergado */
void create_executa_postergado_queue(int *id) {
  *id = msgget(QUEUE_KEY_AT, IPC_CREAT | 0777);
  if( *id < 0 ) {
    printf("error ao criar a fila\n");
    exit(1);
  }
}

/* pega nova mensagem enviada pelo executa_postergado */
void rcv_executa_postergado_msg(int id_fila) {
  msgrcv(id_fila, &msg_received, sizeof(msg_received), 0, 0);
}

/* deixa process_table compartilhada */
void cria_memoria_compartilhada() {
  int process_table_id;
  process_table_id = shmget( PROCESS_TABLE, 16 * sizeof(Process), IPC_CREAT | 0700);
  if (process_table_id < 0) {
    printf("error ao criar memoria compartilhada\n");
    exit(1);
  }
  process_table = (Process *) shmat(process_table_id, 0, 0777);
}

/* cria gerentes e inicializa process_table */
void cria_processos() {
  int i, pid;
  char number[2];
  for (i=0;i<16;i++) {
    pid = fork();
    if (pid == 0) {
      number[0] = (char)i;
      number[1] = '\n';
      execl("./gerente_execucao", "gerente_execucao", number, (char *) 0);
      break;
    } else {
      process_table[i].pid = pid;
      process_table[i].status = 1;
      process_table[i].vizinho[0] = -1;
      process_table[i].vizinho[1] = -1;
      process_table[i].vizinho[2] = -1;
      process_table[i].vizinho[3] = -1;
    }
  }
}

void hypercube() {}

void checa_topoligia(char const topologia[]) {
  if (strcmp(topologia,"hypercube") == 0) {
    hypercube();
  } else if (strcmp(topologia,"torus") == 0) {
    printf("torus\n");
  } else if (strcmp(topologia,"fat_tree") == 0) {
    printf("fat_tree\n");
  } else {
    printf("topologia não existente\n");
    exit(1);
  }
}

void run_delayed() {
  printf("Mensagem recebida\n");
}

int main(int argc, char const *argv[]) {
  int id_fila;

  if (argc !=2 ) {
    printf("Quantidade de argumentos inválida\n");
    exit(1);
  }

  cria_memoria_compartilhada();
  cria_processos();
  checa_topoligia(argv[1]);

  printf("Esperando por mensagem\n");
  while(1) {
    create_executa_postergado_queue(&id_fila);
    rcv_executa_postergado_msg(id_fila);

    printf("Esperando %d segundo para executar: %s\n", msg_received.seconds_to_wait, msg_received.program_name);
    sleep(msg_received.seconds_to_wait);
    run_delayed();
  }
  return 0;
}
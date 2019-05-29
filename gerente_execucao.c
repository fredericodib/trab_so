#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "structs.h"
#include "funcoes_compartilhadas.c"
#include "semaphore.c"

Process *process_table;
Job *current_job;
int ge_id; /* id do gerente */

void recupera_memoria_compartilhada() {
  int process_table_id;
  process_table_id = shmget( PROCESS_TABLE, 16 * sizeof(Process), 0700);
  if (process_table_id < 0) {
    printf("error ao criar memoria compartilhada\n");
    exit(1);
  }
  process_table = (Process *) shmat(process_table_id, 0, 0777);
}

void recupera_memoria_compartilhada_job() {
  int process_job_id;
  process_job_id = shmget( CURRENT_JOB, sizeof(Job), 0700);
  if (process_job_id < 0) {
    printf("error ao criar memoria compartilhada\n");
    exit(1);
  }
  current_job = (Job *) shmat(process_job_id, 0, 0777);
}

int check_todos_os_processos_livres() {
  int i;
  for (i=0;i<16;i++) {
    if (process_table[i].status == 0) {
      return -1;
    }
  }
  return 1;
}

void muda_status(int status) {
  int process_table_sem_id;
  process_table_sem_id = create_semaphore( PROCESS_TABLE_SEM );
  p_sem(process_table_sem_id); /* acessa tabela de processos */
  process_table[ge_id].status = status;
  v_sem(process_table_sem_id); /* libera tabela de processos */
}

void executa_arquivo() {
  printf("\nJob %d começa a executar\n", current_job->job);
  sleep(2);
}

void recebe_sinal(int signal) {
  int process_table_sem_id, process_free_sem_id, process_job_done_sem_id;
  process_free_sem_id = create_semaphore( PROCESS_FREEE_SEM );
  process_job_done_sem_id = create_semaphore( PROCESS_JOB_DONE_SEM );
  process_table_sem_id = create_semaphore( PROCESS_TABLE_SEM );
  muda_status(0);
  executa_arquivo();
  muda_status(1);
  p_sem(process_table_sem_id); /* acessa tabela de processos */
  if (check_todos_os_processos_livres() == 1) {
    v_sem(process_free_sem_id);
    v_sem(process_job_done_sem_id);
  }
  v_sem(process_table_sem_id); /* libera tabela de processos */
  
}

int main(int argc, char const *argv[]) {
  signal(SIGUSR1, recebe_sinal);
  if (argc !=2 ) { /* verifica numero de argumentos */
    printf("Quantidade de argumentos inválida\n");
    exit(1);
  }

  ge_id = (int)argv[1][0]; /* pega id */

  recupera_memoria_compartilhada();
  recupera_memoria_compartilhada_job();

  while(1) {
    sleep(999999);
  }

  return 0;
}
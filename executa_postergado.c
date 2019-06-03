/*
Alunos:
- Frederico Pinheiro Dib - 15/0125925
- Renan Rocha Reboredo - 130/131865
*/
#define QUEUE_KEY_AT 0x1332624
#define JOB_COUNTER 0x194542
#define JOB_COUNTER_SEM 0x7495871

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "structs.h"
#include "semaphore.c"

int *job_counter;

int main(int argc, char const *argv[]) {
  int id_fila, job_counter_sem_id, job_counter_id;
  job_counter_sem_id = create_semaphore( JOB_COUNTER_SEM );

  Message msg_to_send;

  /* falha com a quantidade de argumentos errada */
  if (argc !=3 ) {
    printf("Quantidade de argumentos inválida\n");
    exit(1);
  }

  /* erro em recuperar fila */
  id_fila = msgget(QUEUE_KEY_AT, 0777);
  if( id_fila < 0 ) {
    printf("error ao recuperar a fila\n");
    exit(1);
  }

  job_counter_id = shmget( JOB_COUNTER, sizeof(int), IPC_CREAT | 0700);
  if (job_counter_id < 0) {
    printf("error ao criar memoria compartilhada JOB_COUNTER\n");
    exit(1);
  }
  job_counter = (int *) shmat(job_counter_id, 0, 0777);

  p_sem(job_counter_sem_id);
  msg_to_send.job_number = *job_counter;
  *job_counter = *job_counter + 1;
  v_sem(job_counter_sem_id);

  // prepara a mensagem para colocar na fila
  msg_to_send.pid = getpid();
  strcpy(msg_to_send.program_name, argv[1]);
  msg_to_send.seconds_to_wait = atoi(argv[2]);

  // envia mensagem
  msgsnd(id_fila, &msg_to_send, sizeof(msg_to_send), 0);
  printf("mensagem enviada com sucesso!\n");
  printf("Job: %d; Esperando %d segundo para executar: %s\n\n", msg_to_send.job_number, msg_to_send.seconds_to_wait, msg_to_send.program_name);
  
  return 0;
}
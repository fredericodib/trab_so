/*
Alunos:
- Frederico Pinheiro Dib - 15/0125925
- Renan Rocha Reboredo - 130/131865
*/
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
#include "funcoes_compartilhadas.c"

int main() {
  int scheduler_pid_sem_id, scheduler_pid_id, process_table_id, job_table_id, job_counter_id;
  int *scheduler_pid, *job_counter;
  Process* process_table;
  Job* job_table;
  
  process_table_id = shmget( PROCESS_TABLE, 16 * sizeof(Process), IPC_CREAT | 0700);
  if (process_table_id < 0) {
    printf("error ao criar memoria compartilhada PROCESS_TABLE\n");
    exit(1);
  }
  process_table = (Process *) shmat(process_table_id, 0, 0777);

  job_table_id = shmget( JOB_TABLE, 100 * sizeof(Job), IPC_CREAT | 0700);
  if (job_table_id < 0) {
    printf("error ao criar memoria compartilhada JOB_TABLE\n");
    exit(1);
  }
  job_table = (Job *) shmat(job_table_id, 0, 0777);

  scheduler_pid_sem_id = create_semaphore( SCHEDULER_PID_SEM );
  
  p_sem(scheduler_pid_sem_id);
  scheduler_pid_id = shmget( SCHEDULER_PID, sizeof(int), IPC_CREAT | 0700);
  scheduler_pid = (int*) shmat(scheduler_pid_id, 0, 0777);
  v_sem(scheduler_pid_sem_id);
  kill(*scheduler_pid, SIGTERM);

  job_counter_id = shmget( JOB_COUNTER, sizeof(int), IPC_CREAT | 0700);
  if (job_counter_id < 0) {
    printf("error ao criar memoria compartilhada JOB_COUNTER\n");
    exit(1);
  }
  job_counter = (int *) shmat(job_counter_id, 0, 0777);

  printf("Processo executados ao longo da execucao do escalonador\n");
  for(int i = 1; i < *job_counter; i++) {
    printf("\nJob %d executado! arquivo: %s; tempo de espera: %d; makespan: %ld segundos\n\n", job_table[i].job, job_table[i].exec_file, job_table[i].seconds_to_wait, job_table[i].finish_time - job_table[i].start_time);
  }

  int process_table_sem_id;

  process_table_sem_id = create_semaphore( PROCESS_TABLE_SEM );

  p_sem(process_table_sem_id); /* acessa tabela de processos */
  for (int i=0; i<16; i++) {
    printf("Desligando processo gerenciador com pid: %lu\n", process_table[i].pid);
    kill(process_table[i].pid, SIGKILL);
  }
  v_sem(process_table_sem_id); /* libera tabela de processos */
  printf("Desligando\n");
  exit(0);
}
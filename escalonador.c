#define QUEUE_KEY_AT 0x1332624

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
#include "funcoes_compartilhadas.c"
#include "semaphore.c"
#include "hypercube.c"
#include "fat_tree.c"
#include "torus.c"

Process *process_table;
Job *job_table;
int *job_counter;
Job *current_job;
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

/* deixa process_table, job_counter e current_job compartilhadas */
void cria_memoria_compartilhada() {
  int process_table_id, job_counter_id, current_job_id, job_table_id;

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

  job_counter_id = shmget( JOB_COUNTER, sizeof(int), IPC_CREAT | 0700);
  if (job_counter_id < 0) {
    printf("error ao criar memoria compartilhada JOB_COUNTER\n");
    exit(1);
  }
  job_counter = (int *) shmat(job_counter_id, 0, 0777);
  *job_counter = 1;

  current_job_id = shmget( CURRENT_JOB, sizeof(Job), IPC_CREAT | 0700);
  if (current_job_id < 0) {
    printf("error ao criar memoria compartilhada CURRENT_JOB\n");
    exit(1);
  }
  current_job = (Job *) shmat(current_job_id, 0, 0777);
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
      execl("./gerente_execucao", "gerente_execucao", number, "&", (char *) 0);
      break;
    } else {
      process_table[i].pid = pid;
      process_table[i].status = 1;
      process_table[i].recebeu = 1;
      process_table[i].vizinho[0] = -1;
      process_table[i].vizinho[1] = -1;
      process_table[i].vizinho[2] = -1;
      process_table[i].vizinho[3] = -1;
    }
  }
  printf("gerentes de execução criados\n");
}

void executa_topoligia(char const topologia[]) {
  if (strcmp(topologia,"hypercube") == 0) {
    hypercube();
  } else if (strcmp(topologia,"torus") == 0) {
    torus();
  } else if (strcmp(topologia,"fat_tree") == 0) {
    fat_tree();
  } else {
    printf("topologia não existente\n");
    exit(1);
  }
}

void reseta_semaforos_all() {
  int process_free_sem_id, process_job_done_sem_id, current_job_sem_id,
      process_table_sem_id, job_counter_sem_id, turno_sem_id, scheduler_pid_sem_id;

  process_free_sem_id = create_semaphore( PROCESS_FREEE_SEM );
  process_job_done_sem_id = create_semaphore( PROCESS_JOB_DONE_SEM );
  current_job_sem_id = create_semaphore( CURRENT_JOB_SEM );
  process_table_sem_id = create_semaphore( PROCESS_TABLE_SEM );
  job_counter_sem_id = create_semaphore( JOB_COUNTER_SEM );
  scheduler_pid_sem_id = create_semaphore( SCHEDULER_PID_SEM );
  turno_sem_id = create_semaphore( TURNO_SEM );
  delete_semaphore( scheduler_pid_sem_id );
  delete_semaphore( process_free_sem_id );
  delete_semaphore( process_job_done_sem_id );
  delete_semaphore( current_job_sem_id );
  delete_semaphore( process_table_sem_id );
  delete_semaphore( job_counter_sem_id );
  delete_semaphore( turno_sem_id );
}

void run_delayed() {
  int process_free_sem_id, process_job_done_sem_id, current_job_sem_id, process_table_sem_id, pid, i, turno_sem_id, job_table_id;

  process_free_sem_id = create_semaphore( PROCESS_FREEE_SEM );
  process_job_done_sem_id = create_semaphore( PROCESS_JOB_DONE_SEM );
  current_job_sem_id = create_semaphore( CURRENT_JOB_SEM );
  process_table_sem_id = create_semaphore( PROCESS_TABLE_SEM );
  turno_sem_id = create_semaphore( TURNO_SEM );

  p_sem(turno_sem_id); /* Apenas uma thread do scalonador acessa por vez */
  p_sem(process_free_sem_id); /* se todos os semaforos estao livre, avança*/
  
  p_sem(current_job_sem_id); /* acessa current_job */
  current_job->job = msg_received.job_number;
  strcpy(current_job->exec_file, msg_received.program_name);
  current_job->seconds_to_wait = msg_received.seconds_to_wait;
  v_sem(current_job_sem_id); /* libera current_job */
  
  p_sem(process_table_sem_id); /* acessa tabela de processos */
  pid = process_table[0].pid;
  v_sem(process_table_sem_id); /* libera tabela de processos */

  kill(pid, SIGUSR1);
  
  p_sem(process_job_done_sem_id); /* se o job terminou de ser executado, avança*/

  p_sem(current_job_sem_id); /* acessa current_job */
  printf("\nJob %d executado! arquivo: %s; tempo de espera: %d; makespan: %ld segundos\n\n", current_job->job, current_job->exec_file, current_job->seconds_to_wait, current_job->finish_time - current_job->start_time);
  job_table_id = shmget( JOB_TABLE, 100 * sizeof(Job), IPC_CREAT | 0700);
  if (job_table_id < 0) {
    printf("error ao criar memoria compartilhada JOB_TABLE\n");
    exit(1);
  }
  job_table = (Job *) shmat(job_table_id, 0, 0777);

  job_table[current_job->job].job = current_job->job;
  job_table[current_job->job].finish_time = current_job->finish_time;
  job_table[current_job->job].start_time = current_job->start_time;
  job_table[current_job->job].seconds_to_wait = current_job->seconds_to_wait;
  strcpy(job_table[current_job->job].exec_file, current_job->exec_file);
  v_sem(current_job_sem_id); /* libera current_job */

  for (i=0;i<16;i++) {
    process_table[i].recebeu = 1;
  }
  v_sem(turno_sem_id); /* Apenas uma thread do scalonador acessa por vez */
}

int main(int argc, char const *argv[]) {
  int id_fila, id, process_job_done_id, scheduler_pid_sem_id, scheduler_pid_id, pid;
  int* scheduler_pid;

  reseta_semaforos_all();
  process_job_done_id = create_semaphore( PROCESS_JOB_DONE_SEM );
  scheduler_pid_sem_id = create_semaphore( SCHEDULER_PID_SEM );
  p_sem(process_job_done_id); /* inicializa como 0 */

  if (argc !=2 ) {
    printf("Quantidade de argumentos inválida\n");
    exit(1);
  }

  cria_memoria_compartilhada();
  cria_processos();
  executa_topoligia(argv[1]);

  p_sem(scheduler_pid_sem_id);
  pid = getpid();
  scheduler_pid_id = shmget( SCHEDULER_PID, sizeof(int), IPC_CREAT | 0700);
  scheduler_pid = (int*) shmat(scheduler_pid_id, 0, 0777);
  *scheduler_pid = pid;
  v_sem(scheduler_pid_sem_id);

  printf("Esperando por mensagem\n\n");
  while(1) {
    create_executa_postergado_queue(&id_fila);
    rcv_executa_postergado_msg(id_fila);

    id = fork();
    if (id == 0) {
      sleep(msg_received.seconds_to_wait);
      run_delayed();
      exit(0);
    }
    
  }
  return 0;
}
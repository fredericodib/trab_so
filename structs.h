#include <stdio.h>
#include <string.h>

typedef struct message {
  long pid;
  char program_name[30];
  int seconds_to_wait;
  int job_number;
} Message;

typedef struct job {
  int job;
  char exec_file[30];
  int seconds_to_wait;
  time_t start_time;
  time_t finish_time;
} Job;

typedef struct process {
  long pid; /* Pid... */
  int status; /* Disponivel = 1; Ocupado = 0 */
  int vizinho[4]; /* Ligacoes do gerente  */
  int recebeu; /* nao recebeu sinal = 1; recebeu mensagem = 0 */
} Process;
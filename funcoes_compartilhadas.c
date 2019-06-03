#define PROCESS_TABLE 0x199542
#define JOB_COUNTER 0x194542
#define CURRENT_JOB 0x259542
#define PROCESS_TABLE_SEM 0x7392871
#define PROCESS_FREEE_SEM 0x7492871
#define PROCESS_JOB_DONE_SEM 0x7492873
#define JOB_COUNTER_SEM 0x7495871
#define CURRENT_JOB_SEM 0x7435871
#define TURNO_SEM 0x7464871
#define SCHEDULER_PID_SEM 0x7483871
#define SCHEDULER_PID 0x1991222
#define JOB_TABLE 0x19E542

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "funcoes_compartilhadas.h"

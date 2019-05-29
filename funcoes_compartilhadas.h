#define PROCESS_TABLE 0x199542
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

void recupera_memoria_compartilhada();
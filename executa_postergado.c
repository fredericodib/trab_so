#define QUEUE_KEY_AT 0x1332624

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include "structs.h"

int main(int argc, char const *argv[]) {
  int id_fila;

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

  // prepara a mensagem para colocar na fila
  msg_to_send.pid = getpid();
  strcpy(msg_to_send.program_name, argv[1]);
  msg_to_send.seconds_to_wait = atoi(argv[2]);

  // envia mensagem
  msgsnd(id_fila, &msg_to_send, sizeof(msg_to_send), 0);
  printf("mensagem enviada com sucesso!\n");
  
  return 0;
}
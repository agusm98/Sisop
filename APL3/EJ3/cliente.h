#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//SEMAFORO
#include <semaphore.h>
#include <fcntl.h>
//SIGNAL
#include <signal.h>

#include <unistd.h>

const char* FIFO1 = "/tmp/FIFO1";
const char* FIFO2 = "/tmp/FIFO2";

char salida[500], entrada[80];
int FD_IN, FD_OUT;

sem_t* cli_inst;
sem_t* serv_inst;

#endif // CLIENTE_H_INCLUDED

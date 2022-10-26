#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//SEMAFORO
#include <semaphore.h>
#include <fcntl.h>

#include <unistd.h>

const char* FIFO1 = "/tmp/FIFO1";
const char* FIFO2 = "/tmp/FIFO2";

sem_t* sem_fifo1;
sem_t* sem_fifo2;

#endif // CLIENTE_H_INCLUDED

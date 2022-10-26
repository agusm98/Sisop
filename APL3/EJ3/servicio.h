#ifndef SERVICIO_H_INCLUDED
#define SERVICIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//FIFO
#include <sys/types.h>
#include <sys/stat.h>
//FORK
#include <unistd.h>
//SEMAFORO
#include <semaphore.h>
#include <fcntl.h>

typedef struct {
    int id;
    char descr[30];
    float prec;
    float cost;
    int stock;
} t_producto;

const char* FIFO1 = "/tmp/FIFO1";
const char* FIFO2 = "/tmp/FIFO2";

sem_t* sem_fifo1;
sem_t* sem_fifo2;

int validate(int argc, char* argv[]);

void usage();

int serv_run();

#endif // SERVICIO_H_INCLUDED

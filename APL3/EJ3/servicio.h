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

int validate(int argc, char* argv[]);

void usage();

int serv_run();

int get_params(char* opt);

void action_handler(char* opt, int num, char* msg);
//ACTIONS
void sin_stock(char* msg);
void list_prd(char* msg);
void stock(int num, char* msg);
void repo(int num, char* msg);

#endif // SERVICIO_H_INCLUDED

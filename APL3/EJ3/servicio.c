#include "servicio.h"

int main(int argc, char* argv[])
{
    //validate(argc, argv);

    //INPUT FIFO
    mkfifo(FIFO1, 0666);
    //OUTPUT FIFO
    mkfifo(FIFO2, 0666);

    pid_t pid = fork();

    if(pid == 0)
    {
        //crear semaforos
        sem_fifo1 = sem_open("sem_fifo1", O_CREAT, 0644, 0);
        sem_fifo2 = sem_open("sem_fifo2", O_CREAT, 0644, 0);
        serv_run();
    } else {
        printf("Proceso servidor corriendo en segundo plano!");
    }

    return 0;
}

int validate(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("ERROR: Cantidad de parametros\n");
        printf("Para obtener ayuda utilizar los parametros -h | -help | -?\n");
        exit(-1);
    } else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "-help") == 0) {
        usage();
    }
    return 1;
}

void usage()
{
    printf("\nNOMBRE\n");
    printf("servicio\n");
    printf("SYNOPSIS\n");
    printf("servicio <file>\n");
    printf("EJEMPLO\n");
    printf("./servicio ./Productos\n");
    printf("DESCRIPCION\n");
    printf("Proceso servidor que queda a la escucha de las comandas de algun proceso cliente, para operar sobre el archivo pasado por parametro.\n");
    exit(1);
}

int serv_run()
{
    char cli_opt[15];
    int FD_IN, FD_OUT;

    FD_IN = open(FIFO1, O_RDONLY);
    FD_OUT = open(FIFO2, O_WRONLY);

    do
    {
        //P(FIFO1)
        //sem_wait(sem_fifo1);

        read(FD_IN, cli_opt, 15);
        //close(FD_IN);

        //Handle and write to FIFO2
        //handle_cli(cli_opt);
        printf("FIFO 1 INPUT: %s", cli_opt);

        write(FD_OUT, "Servicio: Mensaje recibido cliente!\n", 37);
        //close(FD_OUT);
        //V(FIFO2)
        //sem_post(sem_fifo2);
    } while (strstr(cli_opt, "QUIT") == NULL);

    close(FD_IN);

    return 0;
}

void handle_cli(char *opt)
{
    //Obtener comanda
    //char* cli_comm = get_opt(opt);
    //Obtener argumento adicional
    //nt cli_arg = get_arg(opt);

    //etc
}

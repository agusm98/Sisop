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
    int FD_IN, FD_OUT, opt_num;
    char text_msg[500];

    FD_IN = open(FIFO1, O_RDONLY);
    FD_OUT = open(FIFO2, O_WRONLY);

    do
    {
        //P(FIFO1)
        //sem_wait(sem_fifo1);

        read(FD_IN, cli_opt, 15);
        //close(FD_IN);
        opt_num = get_params(cli_opt);
        //Handle and write to FIFO2
        //handle_cli(cli_opt);
        printf("OPTION: %s\n", cli_opt);
        printf("NUME: %d\n", opt_num);

        action_handler(cli_opt, opt_num, text_msg);
        printf("Paso action handler text msg %s", text_msg);

        //text_msg =
        //  "ID;DESCRIPCION;PRECIO;COSTO;STOCK;\n1;Harina Blancaflor;70;40;0;\n2;Yerba Mate Cruz de Malta;180;120;100;\n3;Café La Morenita;140;100;53;\n4;Té La Virginia;60;30;0;\n";

        write(FD_OUT, text_msg, sizeof(text_msg));
        //close(FD_OUT);
        //V(FIFO2)
        //sem_post(sem_fifo2);
    } while (strstr(cli_opt, "QUIT") == NULL);

    close(FD_IN);
    close(FD_OUT);
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}

int get_params(char* opt)
{
    //Obtener comanda
    while(*opt != '\0')
    {
        if(*opt == ' ') {
            *opt = '\0';

            ++opt;
            return atoi(opt); //Agusm98 was here!
        }
        ++opt;
    }

    return -1;
}

void sin_stock(char* msg)
{
    strncpy(msg, "SIN_STOCK", 10);
}

void list_prd(char* msg)
{
    strncpy(msg, "LIST", 5);
}
void stock(int num, char* msg)
{
    strncpy(msg, "STOCK", 6);
}

void repo(int num, char* msg)
{
    strncpy(msg, "REPO", 5);
}

void action_handler(char* opt, int num, char* msg)
{
    if(num == -1) {
        if(strcmp(opt, "SIN_STOCK") == 0) printf("No tenemos stock");//sin_stock(msg);
        if(strcmp(opt, "LIST") == 0) printf("No tenemos listado");//list_prd(msg);
    } else {
        if(strcmp(opt, "STOCK") == 0) printf("Ya le dije que no tenemos stock!");// stock(num, msg);
        if(strcmp(opt, "REPO") == 0) printf("reponermo");//repo(num, msg);
    }
}



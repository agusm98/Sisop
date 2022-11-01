#include "servicio.h"

int main(int argc, char* argv[])
{
    validate(argc, argv);

    //INPUT FIFO
    mkfifo(FIFO1, 0666);
    //OUTPUT FIFO
    mkfifo(FIFO2, 0666);

    pid_t pid = fork();

    if(pid == 0)
    {
        //crear semaforos
        serv_run(argv[1]);
    } else {
        printf("Proceso servidor corriendo en segundo plano!\n");
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

int serv_run(char* filename)
{
    char cli_opt[15];
    int FD_IN, FD_OUT, opt_num;
    char text_msg[200] = "";

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

        memset(text_msg, '\0', sizeof(text_msg));

        action_handler(cli_opt, opt_num, filename, text_msg);

        //text_msg =
        //  "ID;DESCRIPCION;PRECIO;COSTO;STOCK;\n1;Harina Blancaflor;70;40;0;\n2;Yerba Mate Cruz de Malta;180;120;100;\n3;Café La Morenita;140;100;53;\n4;Té La Virginia;60;30;0;\n";

        write(FD_OUT, text_msg, 200);
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

void sin_stock(char* msg, char* filename)
{
    char rline[60];
    t_producto prod;
    FILE* fp;
    int prod_found = 0;

    fp = fopen(filename, "r");
    fgets(rline, sizeof(rline), fp);//read header first
    while(fscanf(fp, "%d;%[^;];%d;%d;%d;", &prod.id, prod.descr, &prod.prec, &prod.cost, &prod.stock) != EOF)
    {
        if(prod.stock == 0)
        {
            sprintf(rline, "%d %s $%d\n", prod.id, prod.descr, prod.cost);
            strcat(msg, rline);
            if(!prod_found) prod_found = 1;
         }
    }
    fclose(fp);

    if(!prod_found)
    {
        char err_msg[] = "No se encontraron productos sin stock\n";
        memcpy(msg, err_msg, sizeof(err_msg));
    }
}

void list_prd(char* msg, char* filename)
{
    char rline[60];
    t_producto prod;
    FILE* fp = fopen(filename, "r");
    fgets(rline, sizeof(rline), fp);//read header first
    while(fscanf(fp, "%d;%[^;];%d;%d;%d;", &prod.id, prod.descr, &prod.prec, &prod.cost, &prod.stock) != EOF)
    {
        sprintf(rline, "%d %s $%d\n", prod.id, prod.descr, prod.prec);
        strcat(msg, rline);
    }
    fclose(fp);
    //memcpy(msg, ret_msg, sizeof(ret_msg));
}
void stock(int num, char* msg, char* filename)
{
    char rline[60];
    t_producto prod;
    FILE* fp;
    int prod_found = 0;


    if(num == -1)
    {
        char err_msg[] = "No se pudo ejecutar la opcion STOCK falta id de producto\n";
        memcpy(msg, err_msg, sizeof(err_msg));
    } else {
        fp = fopen(filename, "r");
        fgets(rline, sizeof(rline), fp);//read header first
        while(fscanf(fp, "%d;%[^;];%d;%d;%d;", &prod.id, prod.descr, &prod.prec, &prod.cost, &prod.stock) != EOF)
        {
            if(prod.id == num) {
                sprintf(rline, "%s %du\n", prod.descr, prod.stock);
                strcat(msg, rline);
                prod_found = 1;
                break;
            }
        }
        fclose(fp);

        if(!prod_found)
        {
            char err_msg[] = "No se pudo encontrar el producto\n";
            memcpy(msg, err_msg, sizeof(err_msg));
        }
    }

}

void repo(int num, char* msg, char* filename)
{
    char rline[60];
    t_producto prod;
    FILE* fp;
    int total = 0;

    fp = fopen(filename, "r");
    fgets(rline, sizeof(rline), fp);//read header first
    while(fscanf(fp, "%d;%[^;];%d;%d;%d;", &prod.id, prod.descr, &prod.prec, &prod.cost, &prod.stock) != EOF)
    {
        if(prod.stock == 0)
        {
            total += num * prod.cost;
         }
    }
    fclose(fp);

    if(!total)
    {
        char err_msg[] = "No se pudo calcular la reposicion!\n";
        memcpy(msg, err_msg, sizeof(err_msg));
    } else {
        sprintf(msg, "$%d\n", total);
    }
}

void action_handler(char* opt, int num, char* filename, char* msg)
{
    int action = 0;

    if(strstr(opt, "SIN_STOCK") != NULL)
    {
        sin_stock(msg, filename);
        action = 1;
    }
    if(strstr(opt, "LIST") != NULL)
    {
        list_prd(msg, filename);
        action = 1;
    }

    if(!action && strstr(opt, "STOCK") != NULL)
    {
        stock(num, msg, filename);
        action = 1;
    }

    if(strstr(opt, "REPO") != NULL)
    {
        repo(num, msg, filename);
        action = 1;
    }

    if(!action) {
        char ret_msg[] = "No se ejecuto ninguna opcion [SIN_STOCK, LIST, STOCK, REPO]\n";
        memcpy(msg, ret_msg, sizeof(ret_msg));
    }
}



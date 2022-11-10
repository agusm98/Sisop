#include "cliente.h"

int main(int argc, char* argv[])
{
    signal(SIGINT, SIG_IGN);

    serv_inst = sem_open("serv_inst", 0);

    if (serv_inst == NULL)
    {
        printf("No se encuentra el servicio, cerrando...\n");
        exit(1);
    }

    cli_inst = sem_open("cli_inst", 0);
    if (cli_inst == NULL)
    {
        cli_inst = sem_open("cli_inst", O_CREAT, S_IRUSR | S_IWUSR, 0); // 0600
        printf("Inicia cliente\n");
    } else {
        printf("Ya existe una instancia cliente corriendo!\n");
        exit(1);
    }

    FD_OUT = open(FIFO1, O_WRONLY);
    FD_IN = open(FIFO2, O_RDONLY);

    while (strstr(entrada, "QUIT") == NULL)
    {
        printf("Ingrese opcion [SIN_STOCK, LIST, STOCK, REPO, QUIT]\n");
        fflush(stdin);
		fgets(entrada, 80, stdin);

		entrada[strcspn(entrada, "\r\n")] = 0;

		write(FD_OUT, entrada, strlen(entrada) + 1);

        read(FD_IN, salida, 500);
        printf("Respuesta:\n%s\n", salida);
	}

	sem_close(cli_inst);
	sem_unlink("cli_inst");

    return 0;
}

#include "cliente.h"

int main(int argc, char* argv[])
{
    char salida[500], entrada[80];
    int FD_IN, FD_OUT;
    printf("Inicia cliente\n");

    //Obtener semaforos
    //sem_fifo1 = sem_open("sem_fifo1", 0);
    //sem_fifo2 = sem_open("sem_fifo2", 0);

    FD_OUT = open(FIFO1, O_WRONLY);
    FD_IN = open(FIFO2, O_RDONLY);

    while (strstr(entrada, "QUIT") == NULL)
    {
        fflush(stdin);
		fgets(entrada, 80, stdin);

		entrada[strcspn(entrada, "\r\n")] = 0;

		write(FD_OUT, entrada, strlen(entrada) + 1);

        read(FD_IN, salida, 500);
        printf("Respuesta:\n%s\n", salida);
	}

    return 0;
}

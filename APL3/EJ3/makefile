all: servicio cliente

servicio: servicio.c servicio.h
	gcc -o servicio servicio.c -lrt -lpthread

cliente: cliente.c cliente.h
	gcc -o cliente cliente.c -lrt -lpthread

clean:
	rm servicio cliente

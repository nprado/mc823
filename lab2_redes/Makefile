CC = gcc
CFLAGS = -g -Wall

ALL = materias.o  lab2_server_udp lab2_client_udp client_statistic_2

all: $(ALL)

materias.o: materias.c materias.h

lab2_server_udp: materias.o lab2_server_udp.o

lab2_client_udp: lab2_client_udp.o
client_statistic_2: client_statistic_2.o

clean:
	rm -f *.o *~ $(ALL)


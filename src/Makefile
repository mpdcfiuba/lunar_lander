CC := gcc
CFLAGS := -Wall -pedantic -std=c99 -c -g -O3
LFLAGS := -lm -lSDL3

all: lunar_lander

aux_n_vector_func.o: aux_n_vector_func.c aux_n_vector_func.h
	$(CC) $(CFLAGS) aux_n_vector_func.c

draw_func.o: draw_func.c draw_func.h dictionary.h aux_n_vector_func.h
	$(CC) $(CFLAGS) draw_func.c

dictionary.o: dictionary.c dictionary.h
	$(CC) $(CFLAGS) dictionary.c

caracteres.o: caracteres.c caracteres.h
	$(CC) $(CFLAGS) caracteres.c

naves.o: naves.c naves.h
	$(CC) $(CFLAGS) naves.c

main.o: main.c dictionary.h aux_n_vector_func.h draw_func.h
	$(CC) $(CFLAGS) main.c

lunar_lander: aux_n_vector_func.o draw_func.o dictionary.o caracteres.o naves.o main.o
	$(CC) aux_n_vector_func.o draw_func.o dictionary.o caracteres.o naves.o main.o -o lunar_lander $(LFLAGS)

clean:
	rm *.o

valgrind: lunar_lander
	valgrind --leak-check=full ./lunar_lander

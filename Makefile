# Makefile del proyecto Comunas Anarquistas.
# make          -> compila la simulacion completa (ejecutable: prog)
# make demo     -> compila el demo de estructuras aparte (ejecutable: demo)
# make clean    -> borra los ejecutables y los .o generados

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Archivos .c de la simulacion completa (no incluye DemoEstructuras.c, que tiene su propio main)
FUENTES_SIMULACION = main.c Nodos.c Sociedad.c Indices.c Consumo.c Emergencias.c Trueque.c Busqueda.c PasarDia.c

# Archivos .c que necesita el demo de estructuras (su propio main, mas Nodos y Sociedad para liberar memoria)
FUENTES_DEMO = DemoEstructuras.c Nodos.c Sociedad.c

all: prog

prog: $(FUENTES_SIMULACION)
	$(CC) $(CFLAGS) -o prog $(FUENTES_SIMULACION)

demo: $(FUENTES_DEMO)
	$(CC) $(CFLAGS) -o demo $(FUENTES_DEMO)

clean:
	rm -f prog demo *.o

.PHONY: all demo clean
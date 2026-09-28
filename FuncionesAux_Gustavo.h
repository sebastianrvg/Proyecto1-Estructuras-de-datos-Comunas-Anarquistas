#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include "Nodos.h"

void calcularPuntosRecursos(struct Recursos *lista, int personas, int *suma_puntos, int *cantidad_emergencias);
void calcularPuntosServicios(struct Servicios *lista, int personas, int *suma_puntos, int *cantidad_emergencias);

#endif
#ifndef COMPUTADORA_H
#define COMPUTADORA_H

#include "Nodos.h"

// Funciones de las comunas autonomas (administrado = 0): trueques que hace
// la propia computadora, sin preguntarle nada al usuario ni imprimir los intentos.

int trueque_bienes_computadora(struct Comuna *solicitante, struct Comuna *proveedora, char *nombre_buscado, int cantidad_buscada);
int pedir_a_todas(struct Comuna *solicitante, struct Recursos *recurso, int cantidad);
int bajar_cantidad(int cantidad);
int pedir_recurso_computadora(struct Comuna *solicitante, struct Recursos *recurso, int objetivo);
void trueque_computadora(struct Comuna *inicio, int cantidadComunas);

#endif
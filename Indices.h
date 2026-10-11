#ifndef INDICES_H
#define INDICES_H

#include "Nodos.h"

// Funciones que calculan la necesidad y la satisfacción de las comunas.

float formulaSatisfaccion(struct Comuna *comuna);
float calcularSatisfaccion(struct Comuna *actual, int cantidadComunas);
float calcularNecesidad(struct Comuna *comuna);
void actualizarIndices(struct Comuna *inicio, int cantidadComunas);

#endif

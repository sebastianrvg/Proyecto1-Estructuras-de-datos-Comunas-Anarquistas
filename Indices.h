#ifndef INDICES_H
#define INDICES_H

#include "Nodos.h"

// Funciones que calculan la necesidad y la satisfacción de las comunas.

float formulaSatisfaccion(struct Comuna *comuna);
float calcularSatisfaccion(struct Comuna *actual, int cantidadComunas);
float calcularReciprocidad(struct Comuna *comuna);
float calcularNecesidad(struct Comuna *comuna, int cantidadComunas);
float calcularNecesidadLocal(struct Comuna *comuna);
float valorComunaVecina(struct Comuna *vecina, int tipo);
float promedioVecinas(struct Comuna *actual, int cantidadComunas, int tipo);
void actualizarIndices(struct Comuna *inicio, int cantidadComunas);

#endif
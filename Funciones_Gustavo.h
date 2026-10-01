#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "Nodos.h"

float formulaSatisfaccion(struct Comuna *comuna);
float calcularSatisfaccion(struct Comuna *actual, int cantidadComunas);
float calcularNecesidad(struct Comuna *comuna);
void actualizarIndices(struct Comuna *inicio, int cantidadComunas);
void aplicarConsumo(struct Comuna *comuna);
void aplicarConsumoTodas(struct Comuna *inicio);
void pasarTurno(struct Comuna *inicio, int cantidadComunas);

#endif
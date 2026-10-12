#ifndef EMERGENCIAS_H
#define EMERGENCIAS_H

#include "Nodos.h"

// Funciones que generan y controlan las emergencias de las comunas,
// incluyendo el regalo que otra comuna le puede hacer a la afectada.

void alcanceEmergencia(struct Comuna *comuna, int perdida, char *nombre_emergencia);
int hayRegalo(void);
int regalarRecursos(struct Comuna *comunaAfectada, int regalo);
void generarEmergencia(struct Comuna *actual, int cantidadComunas);
void controlarEmergencias(struct Comuna *inicio, int cantidadComunas, int *turnosHastaEmergencia);
void mostrarEmergencias(struct Comuna *inicio);

#endif
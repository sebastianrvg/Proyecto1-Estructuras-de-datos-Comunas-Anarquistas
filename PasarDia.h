#ifndef PASAR_DIA_H
#define PASAR_DIA_H

#include "Nodos.h"

// Función que avanza un día de la simulación: consumo, índices y emergencias.

void pasarTurno(struct Comuna *inicio, int cantidadComunas, int *turnosHastaEmergencia);

#endif
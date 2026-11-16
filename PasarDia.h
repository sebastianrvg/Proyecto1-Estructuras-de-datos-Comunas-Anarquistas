#ifndef PASAR_DIA_H
#define PASAR_DIA_H

#include "Nodos.h"

// Función que avanza un día de la simulación: consumo, índices, emergencias y disolución de comunas.

/*
Funcionamiento: aplica el consumo de bienes, actualiza los indices, controla emergencias y, al final, revisa si alguna comuna debe disolverse por necesidad muy alta o satisfaccion muy baja y la disuelve si corresponde.
Entradas: inicio (puntero al puntero de cualquier comuna de la lista circular, se actualiza si la que se disuelve era el inicio),
          cantidadComunas (puntero a la cantidad total de comunas, se actualiza si se disuelve alguna),
          turnosHastaEmergencia (puntero al contador de turnos para la proxima emergencia),
          comunaActual (puntero a la comuna administrada seleccionada por el usuario, se actualiza si era la disuelta)
Salidas: ninguna
*/
int pasarTurno(struct Comuna **inicio, int *cantidadComunas, int *turnosHastaEmergencia, struct Comuna **comunaActual);

#endif
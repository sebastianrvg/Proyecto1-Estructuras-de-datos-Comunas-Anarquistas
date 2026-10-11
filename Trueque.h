#ifndef TRUEQUE_H
#define TRUEQUE_H

#include "Nodos.h"

// Funciones que manejan los intercambios (trueques) de bienes y servicios
// entre comunas, incluyendo su validación y el bono de reciprocidad.

int validarTrueque(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, int cantidad);
int calcularBono(int cantidad);
void truequeBienes(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, char *nombre_buscado, int cantidad_buscada);
void truequeServicios(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, char *nombre_buscado, int cantidad_buscada);
void trueque(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, int tipo, char *nombre_buscado, int cantidad_buscada);

#endif

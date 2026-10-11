#ifndef CONSUMO_H
#define CONSUMO_H

#include "Nodos.h"

// Funciones que aplican el consumo de bienes de las personas en cada turno.

void aplicarConsumo(struct Comuna *comuna);
void aplicarConsumoTodas(struct Comuna *inicio);

#endif

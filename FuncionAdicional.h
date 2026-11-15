#ifndef FUNCIONADICIONAL_H
#define FUNCIONADICIONAL_H

#include "Nodos.h"

/*
Funcionalidad adicional: produccion de bienes. Cada servicio produce el bien que
esta en su misma posicion (servicio 1 -> bien 1, servicio 2 -> bien 2, ...).
Cada dia, las personas de cada oficio producen su bien. Lo que se produce sube
o baja segun la necesidad y la satisfaccion de la comuna, y lo que pase del
maximo se le regala a otras comunas.
*/

// Lo que produce cada persona por cada servicio que hay en la simulacion (con mas
// servicios hay menos personas por oficio, asi la produccion total no cambia)
#define PRODUCCION_BASE 2

// Ajuste (en porcentaje) que se suma o se resta segun la necesidad y la satisfaccion
#define AJUSTE_PRODUCCION 20

// Limites de los tres niveles (bajo, medio y alto) de la necesidad y la satisfaccion
#define NIVEL_BAJO 33
#define NIVEL_ALTO 66

int ajustePorNivel(float valor);
struct Recursos *bienEnPosicion(struct Recursos *bienes, int posicion);
int calcularProduccion(struct Comuna *comuna, int trabajadores, int cantidadServicios);
int regalarExcedente(struct Comuna *donante, struct Recursos *bien, int excedente);
void mostrarProduccion(struct Comuna *inicio, int cantidadComunas);
void producirBienes(struct Comuna *inicio, int cantidadComunas);

#endif
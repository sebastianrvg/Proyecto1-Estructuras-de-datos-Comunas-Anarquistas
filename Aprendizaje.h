#ifndef APRENDIZAJE_H
#define APRENDIZAJE_H

#include "Nodos.h"

// Probabilidad (en porcentaje) de que una persona vaya a la biblioteca y lea un libro en un dia
#define PROBABILIDAD_LECTURA 20
// Libros que una persona tiene que leer para aprender un servicio nuevo
#define LIBROS_PARA_APRENDER 3

struct Servicios *elegirServicioNuevo(struct Persona *persona, struct Servicios *servicios);
int aprenderServicio(struct Persona *persona, struct Servicios *servicios);
void leerEnBiblioteca(struct Comuna *inicio, int cantidadComunas);

#endif
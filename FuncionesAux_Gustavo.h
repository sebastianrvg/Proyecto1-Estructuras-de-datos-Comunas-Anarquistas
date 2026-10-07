#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H

#include "Nodos.h"

void calcularPuntosRecursos(struct Recursos *lista, int personas, int *suma_puntos, int *cantidad_emergencias);
void calcularPuntosServicios(struct Servicios *lista, int personas, int *suma_puntos, int *cantidad_emergencias);
void alcanceEmergencia(struct Comuna *comuna, int perdida, char *nombre_emergencia);
struct Recursos *buscarRecurso(struct Recursos *lista, char *nombre);
struct Recursos *encontrarRecursoMasBajo(struct Recursos *lista, char *nombre_excluir);
int puedeDarRecurso(struct Recursos *recurso, int personas, int cantidadARestar);
struct Servicios *buscarServicio(struct Servicios *lista, char *nombre);
struct Servicios *encontrarServicioMasBajo(struct Servicios *lista, char *nombre_excluir);
int puedeDarServicio(struct Servicios *lista, struct Servicios *servicio, int personas, int cantidadARestar);
int contarPersonasPorOficio(struct Persona *lista, int oficio);
void sacarPersonaPorOficio(struct Persona **lista, int oficio, struct Persona **sacada);
void trasladarPersonas(struct Comuna *comunaOrigen, struct Comuna *comunaDestino, int oficio, int cantidad);
void pedirConfirmacion(int *respuesta);
int validarTrueque(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, int cantidad);
int calcularBono(int cantidad);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Nodos.h"

struct Recursos *crearRecurso(char nombre[], int cantidad, int cantidadMaxima) {
    struct Recursos *nuevo;
    nuevo = calloc(1, sizeof(struct Recursos));
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->cantidad = cantidad;
    nuevo->cantidadMaxima = cantidadMaxima;
    nuevo->siguiente = NULL;
    return nuevo;
}

struct Servicios *crearServicio(char nombre[], int cantidad, int cantidadMaxima) {
    struct Servicios *nuevo;
    nuevo = calloc(1, sizeof(struct Servicios));
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->cantidad = cantidad;
    nuevo->cantidadMaxima = cantidadMaxima;
    nuevo->siguiente = NULL;
    return nuevo;
}

struct Comuna *crearNodoCircular(struct Comuna *lista, char nombre[]) {
    struct Comuna *nueva = NULL;
    struct Comuna *actual = NULL;
    nueva = calloc(1, sizeof(struct Comuna));
    strncpy(nueva->nombre, nombre, sizeof(nueva->nombre) - 1);
    nueva->personas = NULL;
    nueva->bienes = NULL;
    nueva->servicios = NULL;
    nueva->necesidad = 0;
    nueva->satisfaccion = 0;
    if (lista == NULL) {
        nueva->siguiente = nueva;
        return nueva;
    }
    actual = lista;
    while (actual->siguiente != lista) {
        actual = actual->siguiente;
    }
    actual->siguiente = nueva;
    nueva->siguiente = lista;
    return lista;
}

struct Comuna *avanzarComunas(struct Comuna *actual, int pasos, int cantidadComunas) {
    int pasos_reales = 0;
    pasos_reales = pasos % cantidadComunas;
    for (int i = 0; i < pasos_reales; i++) {
        actual = actual->siguiente;
    }

    return actual;
}

/*
Funcionamiento: reserva memoria para una nueva persona y copia su nombre.
Entradas: nombre (texto con el nombre de la persona)
Salidas: puntero a la persona creada
*/
struct Persona *crearPersona(char nombre[]) {
    struct Persona *nueva;
 
    nueva = calloc(1, sizeof(struct Persona));
    strncpy(nueva->nombre, nombre, sizeof(nueva->nombre) - 1);
    nueva->siguiente = NULL;
 
    return nueva;
}
 
/*
Funcionamiento: inserta una persona al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (persona a insertar)
Salidas: lista actualizada (la misma, o la nueva persona como inicio si estaba vacía)
*/
struct Persona *insertarPersona(struct Persona *lista, struct Persona *nueva) {
    struct Persona *actual;
 
    if (lista == NULL) {
        return nueva;
    }
 
    actual = lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }
    actual->siguiente = nueva;
 
    return lista;
}
 
/*
Funcionamiento: recorre la lista de personas y cuenta cuántas hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de personas en la lista
*/
int recorrerPersonas(struct Persona *lista) {
    struct Persona *actual;
    int cantidad;

    cantidad = 0;
    actual = lista;
    while (actual != NULL) {
        cantidad = cantidad + 1;
        actual = actual->siguiente;
    }
 
    return cantidad;
}
 
/*
Funcionamiento: recorre la lista de personas e imprime el nombre de cada una.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarPersonas(struct Persona *lista) {
    struct Persona *actual;
    actual = lista;
    while (actual != NULL) {
        printf("%s\n", actual->nombre);
        actual = actual->siguiente;
    }
}
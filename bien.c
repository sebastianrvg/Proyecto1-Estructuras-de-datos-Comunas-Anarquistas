#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bien.h"

/*
Funcionamiento: reserva memoria para un bien nuevo y le carga sus datos
iniciales.
Entradas: nombre (texto con el nombre del bien), existencia (cantidad
inicial), maximo (tope que puede alcanzar), valor (indicador comunitario
inicial)
Salidas: puntero al bien creado
*/
struct Bien *crearBien(char nombre[], int existencia, int maximo, float valor) {
    struct Bien *nueva;

    nueva = calloc(1, sizeof(struct Bien));
    strcpy(nueva->nombre, nombre);
    nueva->existencia = existencia;
    nueva->maximo = maximo;
    nueva->valor = valor;
    nueva->siguiente = NULL;

    return nueva;
}

/*
Funcionamiento: inserta un bien al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (bien a insertar)
Salidas: lista actualizada (la misma, o el bien nuevo como inicio si
estaba vacia)
*/
struct Bien *insertarBien(struct Bien *lista, struct Bien *nueva) {
    struct Bien *actual;

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
Funcionamiento: recorre la lista de bienes y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de bienes en la lista
*/
int recorrerBienes(struct Bien *lista) {
    struct Bien *actual;
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
Funcionamiento: recorre la lista de bienes e imprime los datos de cada uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarBienes(struct Bien *lista) {
    struct Bien *actual;

    actual = lista;
    while (actual != NULL) {
        printf("%s | existencia: %d | maximo: %d | valor: %.2f\n",
               actual->nombre, actual->existencia, actual->maximo, actual->valor);
        actual = actual->siguiente;
    }
}

/*
Funcionamiento: recorre la lista de bienes y libera la memoria de cada
nodo, uno por uno, para no dejar fuga de memoria.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarBienes(struct Bien *lista) {
    struct Bien *actual;
    struct Bien *siguiente;

    actual = lista;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}
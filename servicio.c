#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servicio.h"

/*
Funcionamiento: reserva memoria para un servicio nuevo y le carga sus
datos iniciales.
Entradas: nombre (texto con el nombre del servicio), existencia (cantidad
inicial), maximo (tope que puede alcanzar), valor (indicador comunitario
inicial)
Salidas: puntero al servicio creado
*/
struct Servicio *crearServicio(char nombre[], int existencia, int maximo, float valor) {
    struct Servicio *nueva;

    nueva = calloc(1, sizeof(struct Servicio));
    strcpy(nueva->nombre, nombre);
    nueva->existencia = existencia;
    nueva->maximo = maximo;
    nueva->valor = valor;
    nueva->siguiente = NULL;

    return nueva;
}

/*
Funcionamiento: inserta un servicio al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (servicio a insertar)
Salidas: lista actualizada (la misma, o el servicio nuevo como inicio si
estaba vacia)
*/
struct Servicio *insertarServicio(struct Servicio *lista, struct Servicio *nueva) {
    struct Servicio *actual;

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
Funcionamiento: recorre la lista de servicios y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de servicios en la lista
*/
int recorrerServicios(struct Servicio *lista) {
    struct Servicio *actual;
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
Funcionamiento: recorre la lista de servicios e imprime los datos de cada
uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarServicios(struct Servicio *lista) {
    struct Servicio *actual;

    actual = lista;
    while (actual != NULL) {
        printf("%s | existencia: %d | maximo: %d | valor: %.2f\n",
               actual->nombre, actual->existencia, actual->maximo, actual->valor);
        actual = actual->siguiente;
    }
}

/*
Funcionamiento: recorre la lista de servicios y libera la memoria de cada
nodo, uno por uno, para no dejar fugas de memoria.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarServicios(struct Servicio *lista) {
    struct Servicio *actual;
    struct Servicio *siguiente;

    actual = lista;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

/*
Funcionamiento: abre el archivo de servicios, lee un nombre por linea y
construye la lista enlazada simple del catalogo base. La existencia, el
maximo y el valor quedan en 0 porque todavia no se sabe a que comuna van
a pertenecer ni cuantas personas tiene esa comuna.
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de servicios cargada
*/
struct Servicio *leerServicios(char nombreArchivo[]) {
    FILE *archivo;
    struct Servicio *lista;
    struct Servicio *nueva;
    char linea[50];
    int longitud;

    archivo = fopen(nombreArchivo, "r");
    lista = NULL;

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return lista;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        longitud = strlen(linea);
        if (longitud > 0 && linea[longitud - 1] == '\n') {
            linea[longitud - 1] = '\0';
        }
        nueva = crearServicio(linea, 0, 0, 0);
        lista = insertarServicio(lista, nueva);
    }

    fclose(archivo);

    return lista;
}
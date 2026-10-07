#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Nodos.h"

// ==================== CREAR ==================== 

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

struct Recursos *crearRecurso(char nombre[], int cantidad, int cantidadMaxima) {
    /*
    Funcionamiento: reserva memoria para un nuevo recurso y copia su nombre, cantidad y cantidad máxima.
    Entradas: nombre (texto con el nombre del recurso),
              cantidad (cantidad actual del recurso),
              cantidadMaxima (cantidad máxima que puede ser almacenada)
    Salidas: puntero al recurso creado
    */
    struct Recursos *nuevo;
    nuevo = calloc(1, sizeof(struct Recursos));
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->cantidad = cantidad;
    nuevo->cantidadMaxima = cantidadMaxima;
    nuevo->siguiente = NULL;
    return nuevo;
}

struct Servicios *crearServicio(char nombre[], int cantidad, int cantidadMaxima) {
    /*
    Funcionamiento: reserva memoria para un nuevo servicio y copia su nombre, cantidad y cantidad máxima.
    Entradas: nombre (texto con el nombre del servicio),
              cantidad (cantidad actual del servicio),
              cantidadMaxima (cantidad máxima que puede ser ofrecida)
    Salidas: puntero al servicio creado
    */
    struct Servicios *nuevo;
    nuevo = calloc(1, sizeof(struct Servicios));
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->cantidad = cantidad;
    nuevo->cantidadMaxima = cantidadMaxima;
    nuevo->siguiente = NULL;
    return nuevo;
}

struct Comuna *crearNodoCircular(struct Comuna *lista, char nombre[]) {
    /*
    Funcionamiento: reserva memoria para una nueva comuna y copia su nombre. Inserta la comuna en una lista circular de comunas.
    Entradas: lista (puntero al inicio de la lista circular de comunas),
              nombre (texto con el nombre de la comuna)
    Salidas: puntero al inicio de la lista circular de comunas (puede ser la nueva comuna si la lista estaba vacía)
    */
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

// ==================== RECORRER ==================== 

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
Funcionamiento: recorre la lista de recursos y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de recursos en la lista
*/
int recorrerRecursos(struct Recursos *lista) {
    struct Recursos *actual;
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
Funcionamiento: recorre la lista de servicios y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de servicios en la lista
*/
int recorrerServicios(struct Servicios *lista) {
    struct Servicios *actual;
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
Funcionamiento: recorre la lista circular de comunas y cuenta cuantas
hay. Como es circular, el recorrido se corta cuando se vuelve a llegar
al nodo de inicio, no cuando se encuentra un NULL.
Entradas: lista (inicio de la lista circular)
Salidas: cantidad de comunas en la lista
*/
int recorrerComunas(struct Comuna *lista) {
    struct Comuna *actual;
    int cantidad;

    if (lista == NULL) {
        return 0;
    }

    cantidad = 0;
    actual = lista;
    do {
        cantidad = cantidad + 1;
        actual = actual->siguiente;
    } while (actual != lista);

    return cantidad;
}

struct Comuna *avanzarComunas(struct Comuna *actual, int pasos, int cantidadComunas) {
    int pasos_reales = 0;
    pasos_reales = pasos % cantidadComunas;
    for (int i = 0; i < pasos_reales; i++) {
        actual = actual->siguiente;
    }

    return actual;
}

// ==================== MOSTRAR ==================== 

/*
Funcionamiento: recorre la lista de personas e imprime el nombre de cada una.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarPersonas(struct Persona *lista) {
    struct Persona *actual;
    actual = lista;
    while (actual != NULL) {
        printf("%s | oficio: %d\n", actual->nombre, actual->oficio);
        actual = actual->siguiente;
    }
}

/*
Funcionamiento: recorre la lista de recursos e imprime nombre, cantidad,
cantidadMaxima y emergencias de cada uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarRecursos(struct Recursos *lista) {
    struct Recursos *actual;

    actual = lista;
    while (actual != NULL) {
        printf("%s | cantidad: %d | cantidadMaxima: %d | emergencias: %d\n",
               actual->nombre, actual->cantidad, actual->cantidadMaxima, actual->emergencias);
        actual = actual->siguiente;
    }
}

/*
Funcionamiento: recorre la lista de servicios e imprime nombre, cantidad,
cantidadMaxima y emergencias de cada uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarServicios(struct Servicios *lista) {
    struct Servicios *actual;

    actual = lista;
    while (actual != NULL) {
        printf("%s | cantidad: %d | cantidadMaxima: %d | emergencias: %d | oficio: %d\n",
               actual->nombre, actual->cantidad, actual->cantidadMaxima, actual->emergencias, actual->oficio);
        actual = actual->siguiente;
    }
}

/*
Funcionamiento: recorre la lista circular de comunas e imprime nombre,
necesidad y satisfaccion de cada una. Mismo cuidado que recorrerComunas
con el corte del recorrido.
Entradas: lista (inicio de la lista circular)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarComunas(struct Comuna *lista) {
    struct Comuna *actual;

    if (lista == NULL) {
        return;
    }

    actual = lista;
    do {
        printf("%s | necesidad: %.2f | satisfaccion: %.2f\n",
               actual->nombre, actual->necesidad, actual->satisfaccion);
        actual = actual->siguiente;
    } while (actual != lista);
}

// ==================== INSERTAR Y LEER ==================== 

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
Funcionamiento: inserta un recurso al final de la lista enlazada simple de
recursos.
Entradas: lista (inicio actual de la lista), nueva (recurso a insertar)
Salidas: lista actualizada (la misma, o el recurso nuevo como inicio si
estaba vacía)
*/
struct Recursos *insertarRecurso(struct Recursos *lista, struct Recursos *nueva) {
    struct Recursos *actual;

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
Funcionamiento: inserta un servicio al final de la lista enlazada simple
de servicios.
Entradas: lista (inicio actual de la lista), nueva (servicio a insertar)
Salidas: lista actualizada (la misma, o el servicio nuevo como inicio si
estaba vacía)
*/
struct Servicios *insertarServicio(struct Servicios *lista, struct Servicios *nueva) {
    struct Servicios *actual;

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
Funcionamiento: abre el archivo de personas, lee un nombre por linea y
construye la lista enlazada simple completa. A cada persona le asigna un
oficio al azar entre 1 y cantidadOficios.
Entradas: nombreArchivo (ruta o nombre del archivo a leer),
cantidadOficios (cantidad de servicios activos en la simulacion)
Salidas: puntero al inicio de la lista de personas cargada
*/
struct Persona *leerPersonas(char nombreArchivo[], int cantidadOficios) {
    FILE *archivo;
    struct Persona *lista;
    struct Persona *nueva;
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
        nueva = crearPersona(linea);
        nueva->oficio = (rand() % cantidadOficios) + 1;
        lista = insertarPersona(lista, nueva);
    }

    fclose(archivo);

    return lista;
}

/*
Funcionamiento: abre el archivo de bienes, lee un nombre por linea y
construye la lista enlazada simple del catalogo base de recursos. La
cantidad y la cantidadMaxima quedan en 0 porque todavia no se sabe a que
comuna van a pertenecer ni cuantas personas tiene esa comuna.
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de recursos cargada
*/
struct Recursos *leerRecursos(char nombreArchivo[]) {
    FILE *archivo;
    struct Recursos *lista;
    struct Recursos *nueva;
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
        nueva = crearRecurso(linea, 0, 0);
        lista = insertarRecurso(lista, nueva);
    }

    fclose(archivo);

    return lista;
}

/*
Funcionamiento: abre el archivo de servicios, lee un nombre por linea y
construye la lista enlazada simple del catalogo base de servicios. La
cantidad y la cantidadMaxima quedan en 0 por la misma razon que en
recursos. Ignora las lineas vacias y el oficio de cada servicio es su
posicion en el archivo (empieza en 1).
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de servicios cargada
*/
struct Servicios *leerServicios(char nombreArchivo[]) {
    FILE *archivo;
    struct Servicios *lista;
    struct Servicios *nueva;
    char linea[50];
    int longitud;
    int numero_oficio = 0;

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
            longitud = longitud - 1;
        }
        if (longitud > 0) {
            numero_oficio = numero_oficio + 1;
            nueva = crearServicio(linea, 0, 0);
            nueva->oficio = numero_oficio;
            lista = insertarServicio(lista, nueva);
        }
    }

    fclose(archivo);

    return lista;
}

/*
Funcionamiento: abre el archivo de comunas, lee un nombre por linea y
construye la lista circular usando crearNodoCircular, que ya crea e
inserta el nodo en un solo paso.
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista circular de comunas cargada
*/
struct Comuna *leerComunas(char nombreArchivo[]) {
    FILE *archivo;
    struct Comuna *lista;
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
        lista = crearNodoCircular(lista, linea);
    }

    fclose(archivo);

    return lista;
}

// ==================== LIBERAR ==================== 

/*
Funcionamiento: recorre la lista de recursos y libera la memoria de cada
nodo, uno por uno, para no dejar fugas de memoria.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarRecursos(struct Recursos *lista) {
    struct Recursos *actual;
    struct Recursos *siguiente;

    actual = lista;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

/*
Funcionamiento: recorre la lista de servicios y libera la memoria de cada
nodo, uno por uno, para no dejar fugas de memoria.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarServicios(struct Servicios *lista) {
    struct Servicios *actual;
    struct Servicios *siguiente;

    actual = lista;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}
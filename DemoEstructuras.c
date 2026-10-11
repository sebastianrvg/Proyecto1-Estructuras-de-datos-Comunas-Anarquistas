#include <stdio.h>
#include <stdlib.h>
#include "Nodos.h"
#include "Sociedad.h"

/*
Este archivo no es parte de la simulacion. Es un programa aparte que
demuestra por separado el funcionamiento de las dos estructuras de datos
del proyecto: la lista enlazada simple (se usa para Persona, Recursos y
Servicios) y la lista enlazada circular (se usa para Comuna). Se compila
y se corre solo, sin necesidad de los archivos personas/bienes/servicios/comunas.
*/

void demostrarListaSimple(void) {
    /*
    Funcionamiento: crea una lista enlazada simple de personas, inserta varios
    nodos al final uno por uno, la recorre para mostrarla y contarla, y al
    final libera toda su memoria. Demuestra insertar, recorrer, mostrar y
    liberar sobre una lista simple (sin vuelta al inicio).
    Entradas: ninguna
    Salidas: ninguna (imprime en pantalla)
    */
    struct Persona *lista = NULL;
    struct Persona *nueva = NULL;
    int cantidad = 0;

    printf("Lista enlazada simple (Persona)\n");

    // Insertar cuatro personas, una por una, siempre al final de la lista
    nueva = crearPersona("Anacleta");
    lista = insertarPersona(lista, nueva);

    nueva = crearPersona("Gustavo");
    lista = insertarPersona(lista, nueva);

    nueva = crearPersona("Petronila");
    lista = insertarPersona(lista, nueva);

    nueva = crearPersona("Sebastian");
    lista = insertarPersona(lista, nueva);

    printf("Lista despues de insertar 4 personas:\n");
    mostrarPersonas(lista);

    cantidad = recorrerPersonas(lista);
    printf("Cantidad de personas en la lista: %d\n", cantidad);

    // Liberar toda la memoria de la lista antes de terminar
    liberarPersonas(lista);
    printf("Lista liberada.\n\n");
}

void demostrarListaCircular(void) {
    /*
    Funcionamiento: crea una lista enlazada circular de comunas, inserta
    varios nodos, la recorre dos vuelts completas usando avanzarComunas para
    demostrar que el último nodo apunta de regreso al primero (y no a NULL),
    y al final libera toda su memoria junto con lo que tenga cada comuna
    adentro. Demuestra insertar, recorrer, mostrar y liberar sobre una lista
    circular.
    Entradas: ninguna
    Salidas: ninguna (imprime)
    */
    struct Comuna *lista = NULL;
    struct Comuna *actual = NULL;
    int cantidad = 0;

    printf("Lista enlazada circular (Comuna)\n");

    // Insertar tres comunas; crearNodoCircular ya las inserta en la lista circular
    lista = crearNodoCircular(lista, "Comuna A");
    lista = crearNodoCircular(lista, "Comuna B");
    lista = crearNodoCircular(lista, "Comuna C");

    cantidad = recorrerComunas(lista);
    printf("Cantidad de comunas en la lista: %d\n", cantidad);

    printf("Primera vuelta completa (A, B, C)\n");
    actual = lista;
    for (int i = 0; i < cantidad; i++) {
        printf("  %s\n", actual->nombre);
        actual = actual->siguiente;
    }

    // Seguir avanzando mas pasos de los que hay comunas, para demostrar que
    // la lista es circular: no llega a NULL, sino que vuelve al inicio
    printf("Dos pasos mas alla del final (A, B)\n");
    printf("  %s\n", actual->nombre);
    actual = actual->siguiente;
    printf("  %s\n", actual->nombre);

    // avanzarComunas tambien puede saltar directo a una posicion cualquiera
    actual = avanzarComunas(lista, 5, cantidad); // 5 pasos, con 3 comunas da la vuelta y cae en la posicion 2 (Comuna C)
    printf("avanzarComunas(lista, 5, %d) cae en : %s\n", cantidad, actual->nombre);

    // Liberar toda la memoria de la lista circular (y de cualquier persona,
    // bien o servicio que tuviera cada comuna adentro, aunque aqui no tienen)
    liberarComunas(lista);
    printf("Lista liberada.\n\n");
}

int main(void) {
    demostrarListaSimple();
    demoatrarListaCircular();

    return 0;
}

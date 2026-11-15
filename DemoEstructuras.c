#include <stdio.h>
#include <stdlib.h>
#include "Nodos.h"
#include "Sociedad.h"
#include "Chaflaneado.h"
#include "Arbol.h"

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

void demostrarArbolChaflaneado(void) {
    /*
    Funcionamiento: demuestra el arbol chaflaneado: inserta cinco textos y muestra el arbol despues de cada uno, busca un texto que esta abajo (para ver las rotaciones), busca uno que no existe y vuelve a buscar el primero
    Entradas: ninguna
    Salidas: ninguna (imprime en pantalla)
    */
    struct NodoChaflaneado *raiz = NULL;
    char *textos[5] = {"mar", "luna", "sol", "rio", "viento"};
    char *busquedas[3] = {"luna", "casa", "mar"};
    int encontrado = 0;

    printf("=== Arbol chaflaneado ===\n");
    for (int i = 0; i < 5; i++) {
        printf("Insertar \"%s\":\n", textos[i]);
        raiz = insertarTexto(raiz, textos[i], agregarNombreLibro(NULL, "libro.txt"));
        mostrarArbolChaflaneado(raiz, 0);
        printf("\n");
    }

    for (int i = 0; i < 3; i++) {
        printf("Buscar \"%s\":\n", busquedas[i]);
        raiz = buscarTexto(raiz, busquedas[i], &encontrado);
        if (encontrado == 1) {
            printf("  Encontrado, ahora es la raiz\n");
        } else {
            printf("  No esta; queda en la raiz \"%s\", la mas cercana.\n", raiz->clave);
        }
        mostrarArbolChaflaneado(raiz, 0);
        printf("\n");
    }

    liberarChaflaneado(raiz);
    printf("Arbol liberado.\n\n");
}

void demostrarArbolAvl(void) {
    /*
    Funcionamiento: demuestra el arbol AVL insertando nombres en un orden
    pensado para forzar, una por una, las 4 rotaciones de balanceo (simple
    izquierda, simple derecha, izquierda-derecha y derecha-izquierda),
    mostrando el arbol despues de cada insercion para ver cuando rota. Al
    final hace una busqueda exacta de un nombre que esta y de uno que no.
    Entradas: ninguna
    Salidas: ninguna (imprime en pantalla)
    */
    struct NodoAvl *raiz = NULL;
    struct NodoAvl *encontrado = NULL;
    int i = 0;

    // c, b, a: al insertar "a" el nodo "c" queda con factor de balance 2
    // (pesa a la izquierda), se corrige con una rotacion simple derecha
    char *provocaDerecha[3] = {"c", "b", "a"};

    // d, e, f: al insertar "f" el nodo "d" pesa a la derecha, se corrige
    // con una rotacion simple izquierda
    char *provocaIzquierda[3] = {"d", "e", "f"};

    // aa, ab: "b" queda pesando a la izquierda pero por el hijo derecho de
    // "aa", asi que primero hay que rotar "aa" a la izquierda y despues
    // rotar "b" a la derecha (rotacion doble izquierda-derecha)
    char *provocaIzquierdaDerecha[2] = {"aa", "ab"};

    // ff, fe: mismo caso que el anterior pero al espejo (rotacion doble
    // derecha-izquierda)
    char *provocaDerechaIzquierda[2] = {"ff", "fe"};

    printf("=== Arbol AVL ===\n");

    printf("Forzando rotacion simple derecha:\n");
    for (i = 0; i < 3; i++) {
        raiz = insertar_avl(raiz, provocaDerecha[i]);
        printf("Insertar \"%s\":\n", provocaDerecha[i]);
        mostrar_arbol_avl(raiz, 0);
    }
    printf("\n");

    printf("Forzando rotacion simple izquierda:\n");
    for (i = 0; i < 3; i++) {
        raiz = insertar_avl(raiz, provocaIzquierda[i]);
        printf("Insertar \"%s\":\n", provocaIzquierda[i]);
        mostrar_arbol_avl(raiz, 0);
    }
    printf("\n");

    printf("Forzando rotacion doble izquierda-derecha:\n");
    for (i = 0; i < 2; i++) {
        raiz = insertar_avl(raiz, provocaIzquierdaDerecha[i]);
        printf("Insertar \"%s\":\n", provocaIzquierdaDerecha[i]);
        mostrar_arbol_avl(raiz, 0);
    }
    printf("\n");

    printf("Forzando rotacion doble derecha-izquierda:\n");
    for (i = 0; i < 2; i++) {
        raiz = insertar_avl(raiz, provocaDerechaIzquierda[i]);
        printf("Insertar \"%s\":\n", provocaDerechaIzquierda[i]);
        mostrar_arbol_avl(raiz, 0);
    }
    printf("\n");

    printf("Insertar \"c\" otra vez (ya existe, no se duplica):\n");
    raiz = insertar_avl(raiz, "c");
    mostrar_arbol_avl(raiz, 0);
    printf("\n");

    encontrado = buscar_avl(raiz, "aa");
    printf("Buscar \"aa\": %s\n", encontrado != NULL ? "encontrado" : "no encontrado");

    encontrado = buscar_avl(raiz, "zzz");
    printf("Buscar \"zzz\": %s\n", encontrado != NULL ? "encontrado" : "no encontrado");

    liberar_avl(raiz);
    printf("Arbol liberado.\n\n");
}

int main(void) {
    demostrarListaSimple();
    demostrarListaCircular();
    demostrarArbolChaflaneado();
    demostrarArbolAvl();

    return 0;
}
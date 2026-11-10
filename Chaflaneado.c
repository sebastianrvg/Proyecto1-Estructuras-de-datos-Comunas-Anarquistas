#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaflaneado.h"

struct NombreLibro *agregarNombreLibro(struct NombreLibro *lista, char nombre[]) {
    /*
    Funcionamiento: crea un nombre de libro y lo agrega al final de la lista enlazada simple
    Entradas: lista (inicio de la lista, puede ser NULL), nombre (nombre del libro)
    Salidas: lista actualizada (la misma, o el nuevo nombre como inicio si estaba vacia)
    */
    struct NombreLibro *nuevo = calloc(1, sizeof(struct NombreLibro));
    struct NombreLibro *actual = lista;

    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->siguiente = NULL;

    if (lista == NULL) {
        return nuevo;
    }

    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevo;

    return lista;
}

struct NodoChaflaneado *crearNodoChaflaneado(char clave[], struct NombreLibro *libros) {
    /*
    Funcionamiento: reserva memoria para un nodo del arbol chaflaneado, sin hijos
    Entradas: clave (texto buscado), libros (lista de libros que lo contienen)
    Salidas: puntero al nodo creado
    */
    struct NodoChaflaneado *nuevo = calloc(1, sizeof(struct NodoChaflaneado));

    strncpy(nuevo->clave, clave, sizeof(nuevo->clave) - 1);
    nuevo->libros = libros;
    nuevo->izquierdo = NULL;
    nuevo->derecho = NULL;

    return nuevo;
}

struct NodoChaflaneado *rotarDerecha(struct NodoChaflaneado *x) {
    /*
    Funcionamiento: rotacion a la derecha. El hijo izquierdo de x sube y x pasa a ser su hijo derecho
    Entradas: x (nodo que baja, debe tener hijo izquierdo)
    Salidas: el nodo que queda arriba (el antiguo hijo izquierdo)
    */
    struct NodoChaflaneado *y = x->izquierdo;

    printf("  Rotacion a la derecha: %s sube y %s baja\n", y->clave, x->clave);
    x->izquierdo = y->derecho;
    y->derecho = x;

    return y;
}

struct NodoChaflaneado *rotarIzquierda(struct NodoChaflaneado *x) {
    /*
    Funcionamiento: rotacion a la izquierda. El hijo derecho de x sube y x pasa a ser su hijo izquierdo
    Entradas: x (nodo que baja, debe tener hijo derecho)
    Salidas: el nodo que queda arriba (el antiguo hijo derecho)
    */
    struct NodoChaflaneado *y = x->derecho;

    printf("  Rotacion a la izquierda: %s sube y %s baja\n", y->clave, x->clave);
    x->derecho = y->izquierdo;
    y->izquierdo = x;

    return y;
}

struct NodoChaflaneado *chaflanear(struct NodoChaflaneado *raiz, char clave[]) {
    /*
    Funcionamiento: sube a la raiz el nodo con esa clave (o el ultimo nodo que se visito si no existe) usando rotaciones: zig (un paso), zig-zig (dos pasos del mismo lado) y zig-zag (dos pasos de lados distintos). Imprime cada rotacion
    Entradas: raiz (raiz del arbol o subarbol), clave (texto que se quiere subir)
    Salidas: nueva raiz del arbol
    */
    int comparacion = 0; // Clave contra la raiz
    int comparacion_hijo = 0; // Clave contra el hijo de la raiz

    if (raiz == NULL) {
        return NULL;
    }

    comparacion = strcmp(clave, raiz->clave);
    if (comparacion == 0) {
        return raiz;
    }

    if (comparacion < 0) {
        if (raiz->izquierdo == NULL) {
            return raiz;
        }
        comparacion_hijo = strcmp(clave, raiz->izquierdo->clave);
        if (comparacion_hijo < 0) {
            raiz->izquierdo->izquierdo = chaflanear(raiz->izquierdo->izquierdo, clave);
            if (raiz->izquierdo->izquierdo == NULL) {
                printf("  Zig (izquierda)\n"); // No hay nieto: solo queda un paso
            } else {
                printf("  Zig-zig (izquierda-izquierda)\n");
            }
            raiz = rotarDerecha(raiz);
        } else if (comparacion_hijo > 0) {
            raiz->izquierdo->derecho = chaflanear(raiz->izquierdo->derecho, clave);
            if (raiz->izquierdo->derecho == NULL) {
                printf("  Zig (izquierda)\n"); // No hay nieto: solo queda un paso
            } else {
                printf("  Zig-zag (izquierda-derecha)\n");
                raiz->izquierdo = rotarIzquierda(raiz->izquierdo);
            }
        } else {
            printf("  Zig (izquierda)\n");
        }
        if (raiz->izquierdo == NULL) {
            return raiz;
        }
        return rotarDerecha(raiz);
    }

    if (raiz->derecho == NULL) {
        return raiz;
    }
    comparacion_hijo = strcmp(clave, raiz->derecho->clave);
    if (comparacion_hijo > 0) {
        raiz->derecho->derecho = chaflanear(raiz->derecho->derecho, clave);
        if (raiz->derecho->derecho == NULL) {
            printf("  Zig (derecha)\n"); // No hay nieto: solo queda un paso
        } else {
            printf("  Zig-zig (derecha-derecha)\n");
        }
        raiz = rotarIzquierda(raiz);
    } else if (comparacion_hijo < 0) {
        raiz->derecho->izquierdo = chaflanear(raiz->derecho->izquierdo, clave);
        if (raiz->derecho->izquierdo == NULL) {
            printf("  Zig (derecha)\n"); // No hay nieto: solo queda un paso
        } else {
            printf("  Zig-zag (derecha-izquierda)\n");
            raiz->derecho = rotarDerecha(raiz->derecho);
        }
    } else {
        printf("  Zig (derecha)\n");
    }
    if (raiz->derecho == NULL) {
        return raiz;
    }
    return rotarIzquierda(raiz);
}

struct NodoChaflaneado *insertarTexto(struct NodoChaflaneado *raiz, char clave[], struct NombreLibro *libros) {
    /*
    Funcionamiento: inserta una clave y la deja en la raiz. Primero chaflanea la clave mas cercana a la raiz y luego parte el arbol en dos alrededor de la nueva. Si la clave ya existe reemplaza su lista de libros
    Entradas: raiz (raiz del arbol), clave (texto buscado), libros (lista de libros que lo contienen, el arbol queda con ella)
    Salidas: nueva raiz del arbol (el nodo de la clave)
    */
    struct NodoChaflaneado *nuevo = NULL;
    int comparacion = 0;

    if (raiz == NULL) {
        return crearNodoChaflaneado(clave, libros);
    }

    raiz = chaflanear(raiz, clave);
    comparacion = strcmp(clave, raiz->clave);

    if (comparacion == 0) {
        liberarNombresLibro(raiz->libros);
        raiz->libros = libros;
        return raiz;
    }

    nuevo = crearNodoChaflaneado(clave, libros);
    if (comparacion < 0) {
        nuevo->derecho = raiz;
        nuevo->izquierdo = raiz->izquierdo;
        raiz->izquierdo = NULL;
    } else {
        nuevo->izquierdo = raiz;
        nuevo->derecho = raiz->derecho;
        raiz->derecho = NULL;
    }

    return nuevo;
}

struct NodoChaflaneado *buscarTexto(struct NodoChaflaneado *raiz, char clave[], int *encontrado) {
    /*
    Funcionamiento: busca una clave chaflaneando el arbol. Si existe queda en la raiz; si no, queda en la raiz el ultimo nodo que se visito
    Entradas: raiz (raiz del arbol), clave (texto buscado), encontrado (se llena con 1 si la clave existe y con 0 si no)
    Salidas: nueva raiz del arbol (si se encontro, su lista de libros es raiz->libros)
    */
    raiz = chaflanear(raiz, clave);

    if (raiz != NULL && strcmp(raiz->clave, clave) == 0) {
        *encontrado = 1;
    } else {
        *encontrado = 0;
    }

    return raiz;
}

void mostrarArbolChaflaneado(struct NodoChaflaneado *raiz, int nivel) {
    /*
    Funcionamiento: imprime el arbol de lado: la raiz a la izquierda, el subarbol derecho arriba y el izquierdo abajo, con una sangria por nivel
    Entradas: raiz (raiz del arbol o subarbol), nivel (profundidad del nodo, empieza en 0)
    Salidas: ninguna (imprime en pantalla)
    */
    if (raiz == NULL) {
        return;
    }

    mostrarArbolChaflaneado(raiz->derecho, nivel + 1);
    printf("%*s%s\n", nivel * 6, "", raiz->clave);
    mostrarArbolChaflaneado(raiz->izquierdo, nivel + 1);
}

void liberarNombresLibro(struct NombreLibro *lista) {
    /*
    Funcionamiento: libera la memoria de cada nombre de la lista, uno por uno
    Entradas: lista (inicio de la lista)
    Salidas: ninguna (libera memoria)
    */
    struct NombreLibro *siguiente = NULL;

    while (lista != NULL) {
        siguiente = lista->siguiente;
        free(lista);
        lista = siguiente;
    }
}

void liberarChaflaneado(struct NodoChaflaneado *raiz) {
    /*
    Funcionamiento: libera todo el arbol, incluyendo la lista de libros de cada nodo
    Entradas: raiz (raiz del arbol o subarbol)
    Salidas: ninguna (libera memoria)
    */
    if (raiz == NULL) {
        return;
    }

    liberarChaflaneado(raiz->izquierdo);
    liberarChaflaneado(raiz->derecho);
    liberarNombresLibro(raiz->libros);
    free(raiz);
}
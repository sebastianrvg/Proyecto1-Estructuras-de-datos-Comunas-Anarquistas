#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arbol.h"

int altura_avl(struct NodoAvl *nodo) {
    /*
    Funcionamiento: da la altura de un nodo, tratando un arbol vacio como altura 0
    Entradas: nodo (puede ser NULL)
    Salidas: altura del nodo
    */
    if (nodo == NULL) {
        return 0;
    }
    return nodo->altura;
}

static int mayor(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int factor_balance_avl(struct NodoAvl *nodo) {
    /*
    Funcionamiento: calcula la diferencia entre la altura del subarbol izquierdo
    y el derecho de un nodo. Positivo significa que pesa mas a la izquierda,
    negativo que pesa mas a la derecha
    Entradas: nodo (puede ser NULL)
    Salidas: factor de balance del nodo
    */
    if (nodo == NULL) {
        return 0;
    }
    return altura_avl(nodo->izquierdo) - altura_avl(nodo->derecho);
}

struct NodoAvl *crear_nodo_avl(char nombre[]) {
    /*
    Funcionamiento: reserva memoria para un nodo del arbol AVL, sin hijos y con
    altura 1
    Entradas: nombre (nombre del libro, es la clave)
    Salidas: puntero al nodo creado
    */
    struct NodoAvl *nuevo = calloc(1, sizeof(struct NodoAvl));

    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre) - 1);
    nuevo->altura = 1;
    nuevo->izquierdo = NULL;
    nuevo->derecho = NULL;

    return nuevo;
}

struct NodoAvl *rotar_derecha_avl(struct NodoAvl *nodo) {
    /*
    Funcionamiento: rota un nodo hacia la derecha, subiendo su hijo izquierdo.
    Se usa cuando un nodo pesa de mas hacia la izquierda
    Entradas: nodo (nodo desbalanceado, no puede ser NULL ni tener izquierdo NULL)
    Salidas: puntero al nodo que queda como nueva raiz de ese subarbol
    */
    struct NodoAvl *nuevaRaiz = nodo->izquierdo;
    struct NodoAvl *hijoTraspasado = nuevaRaiz->derecho;

    nuevaRaiz->derecho = nodo;
    nodo->izquierdo = hijoTraspasado;

    nodo->altura = mayor(altura_avl(nodo->izquierdo), altura_avl(nodo->derecho)) + 1;
    nuevaRaiz->altura = mayor(altura_avl(nuevaRaiz->izquierdo), altura_avl(nuevaRaiz->derecho)) + 1;

    return nuevaRaiz;
}

struct NodoAvl *rotar_izquierda_avl(struct NodoAvl *nodo) {
    /*
    Funcionamiento: rota un nodo hacia la izquierda, subiendo su hijo derecho.
    Se usa cuando un nodo pesa de mas hacia la derecha
    Entradas: nodo (nodo desbalanceado, no puede ser NULL ni tener derecho NULL)
    Salidas: puntero al nodo que queda como nueva raiz de ese subarbol
    */
    struct NodoAvl *nuevaRaiz = nodo->derecho;
    struct NodoAvl *hijoTraspasado = nuevaRaiz->izquierdo;

    nuevaRaiz->izquierdo = nodo;
    nodo->derecho = hijoTraspasado;

    nodo->altura = mayor(altura_avl(nodo->izquierdo), altura_avl(nodo->derecho)) + 1;
    nuevaRaiz->altura = mayor(altura_avl(nuevaRaiz->izquierdo), altura_avl(nuevaRaiz->derecho)) + 1;

    return nuevaRaiz;
}

struct NodoAvl *insertar_avl(struct NodoAvl *raiz, char nombre[]) {
    /*
    Funcionamiento: inserta un nombre en el arbol AVL manteniendolo ordenado
    alfabeticamente y balanceado. Si el nombre ya esta en el arbol, no lo
    duplica. Despues de insertar, revisa el factor de balance de cada nodo en
    el camino de vuelta y aplica la rotacion que corresponda (simple izquierda,
    simple derecha, izquierda-derecha o derecha-izquierda)
    Entradas: raiz (raiz del arbol, puede ser NULL), nombre (nombre a insertar)
    Salidas: puntero a la nueva raiz del arbol (puede cambiar tras una rotacion)
    */
    int comparacion;
    int balance;

    if (raiz == NULL) {
        return crear_nodo_avl(nombre);
    }

    comparacion = strcmp(nombre, raiz->nombre);

    if (comparacion < 0) {
        raiz->izquierdo = insertar_avl(raiz->izquierdo, nombre);
    } else if (comparacion > 0) {
        raiz->derecho = insertar_avl(raiz->derecho, nombre);
    } else {
        return raiz; // ya existe, no se duplica
    }

    raiz->altura = mayor(altura_avl(raiz->izquierdo), altura_avl(raiz->derecho)) + 1;
    balance = factor_balance_avl(raiz);

    if (balance > 1 && factor_balance_avl(raiz->izquierdo) >= 0) {
        // caso izquierda-izquierda
        return rotar_derecha_avl(raiz);
    }

    if (balance > 1 && factor_balance_avl(raiz->izquierdo) < 0) {
        // caso izquierda-derecha
        raiz->izquierdo = rotar_izquierda_avl(raiz->izquierdo);
        return rotar_derecha_avl(raiz);
    }

    if (balance < -1 && factor_balance_avl(raiz->derecho) <= 0) {
        // caso derecha-derecha
        return rotar_izquierda_avl(raiz);
    }

    if (balance < -1 && factor_balance_avl(raiz->derecho) > 0) {
        // caso derecha-izquierda
        raiz->derecho = rotar_derecha_avl(raiz->derecho);
        return rotar_izquierda_avl(raiz);
    }

    return raiz;
}

struct NodoAvl *buscar_avl(struct NodoAvl *raiz, char nombre[]) {
    /*
    Funcionamiento: busca un nombre exacto en el arbol AVL
    Entradas: raiz (raiz del arbol, puede ser NULL), nombre (nombre buscado)
    Salidas: puntero al nodo encontrado, o NULL si no esta en el arbol
    */
    int comparacion;

    if (raiz == NULL) {
        return NULL;
    }

    comparacion = strcmp(nombre, raiz->nombre);

    if (comparacion == 0) {
        return raiz;
    }

    if (comparacion < 0) {
        return buscar_avl(raiz->izquierdo, nombre);
    }

    return buscar_avl(raiz->derecho, nombre);
}

void mostrar_arbol_avl(struct NodoAvl *raiz, int nivel) {
    /*
    Funcionamiento: imprime el arbol de lado (derecho arriba, izquierdo abajo),
    sangrando segun el nivel, para poder revisar visualmente que esta balanceado
    Entradas: raiz (raiz del arbol, puede ser NULL), nivel (nivel actual, se llama
    con 0 desde afuera)
    Salidas: ninguna, solo imprime en pantalla
    */
    int i;

    if (raiz == NULL) {
        return;
    }

    mostrar_arbol_avl(raiz->derecho, nivel + 1);

    for (i = 0; i < nivel; i++) {
        printf("    ");
    }
    printf("%s (altura %d)\n", raiz->nombre, raiz->altura);

    mostrar_arbol_avl(raiz->izquierdo, nivel + 1);
}

void liberar_avl(struct NodoAvl *raiz) {
    /*
    Funcionamiento: libera todos los nodos del arbol AVL, recorriendolo en
    postorden para liberar primero los hijos
    Entradas: raiz (raiz del arbol, puede ser NULL)
    Salidas: ninguna
    */
    if (raiz == NULL) {
        return;
    }

    liberar_avl(raiz->izquierdo);
    liberar_avl(raiz->derecho);
    free(raiz);
}

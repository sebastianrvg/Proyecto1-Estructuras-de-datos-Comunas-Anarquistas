#ifndef ARBOL_H
#define ARBOL_H

/*
Arbol AVL. Se usa para buscar un libro por su nombre exacto en tiempo
logaritmico. Cada vez que se inserta un nodo, el arbol se reequilibra con
rotaciones para que la diferencia de alturas entre los dos subarboles de
cualquier nodo nunca sea mayor a 1.
*/

struct NodoAvl {
    char nombre[100]; // Nombre del libro (archivo), es la clave de orden
    int altura;
    struct NodoAvl *izquierdo;
    struct NodoAvl *derecho;
};

int altura_avl(struct NodoAvl *nodo);
int factor_balance_avl(struct NodoAvl *nodo);
struct NodoAvl *crear_nodo_avl(char nombre[]);
struct NodoAvl *rotar_derecha_avl(struct NodoAvl *nodo);
struct NodoAvl *rotar_izquierda_avl(struct NodoAvl *nodo);
struct NodoAvl *insertar_avl(struct NodoAvl *raiz, char nombre[]);
struct NodoAvl *buscar_avl(struct NodoAvl *raiz, char nombre[]);
void mostrar_arbol_avl(struct NodoAvl *raiz, int nivel);
void liberar_avl(struct NodoAvl *raiz);

#endif

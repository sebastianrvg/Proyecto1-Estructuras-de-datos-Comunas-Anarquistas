#ifndef CHAFLANEADO_H
#define CHAFLANEADO_H

/*
Arbol chaflaneado (splay). Se usa como cache de las busquedas de texto: la clave es
el texto buscado y el valor es la lista de libros que lo contienen.
Cada vez que se busca o se inserta una clave, el arbol la sube hasta la raiz con
rotaciones, asi lo que se busca mas seguido queda cerca de la raiz.
*/

struct NombreLibro {
    char nombre[100]; // Nombre del libro (archivo)
    struct NombreLibro *siguiente;
};

struct NodoChaflaneado {
    char clave[100]; // Texto buscado
    struct NombreLibro *libros; // Libros que contienen el texto
    struct NodoChaflaneado *izquierdo;
    struct NodoChaflaneado *derecho;
};

struct NombreLibro *agregarNombreLibro(struct NombreLibro *lista, char nombre[]);
struct NodoChaflaneado *chaflanear(struct NodoChaflaneado *raiz, char clave[]);
struct NodoChaflaneado *insertarTexto(struct NodoChaflaneado *raiz, char clave[], struct NombreLibro *libros);
struct NodoChaflaneado *buscarTexto(struct NodoChaflaneado *raiz, char clave[], int *encontrado);
void mostrarArbolChaflaneado(struct NodoChaflaneado *raiz, int nivel);
void liberarNombresLibro(struct NombreLibro *lista);
void liberarChaflaneado(struct NodoChaflaneado *raiz);

#endif
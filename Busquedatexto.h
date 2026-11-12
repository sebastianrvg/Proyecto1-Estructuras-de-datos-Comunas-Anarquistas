#ifndef BUSQUEDATEXTO_H
#define BUSQUEDATEXTO_H

#include "Chaflaneado.h"

/*
Busqueda de texto dentro de los libros. Primero se busca en el arbol chaflaneado,
que funciona como cache; si el texto no esta, se busca en los archivos y el
resultado se guarda en el arbol para la proxima vez.
*/

struct NombreLibro *leerNombresLibros(char nombreArchivo[]);
int libroContieneTexto(char nombreLibro[], char texto[]);
struct NombreLibro *buscarEnLibros(struct NombreLibro *libros, char texto[]);
struct NodoChaflaneado *buscarTextoConCache(struct NodoChaflaneado *cache, struct NombreLibro *libros, char texto[], struct NombreLibro **resultado);
void mostrarNombresLibro(struct NombreLibro *lista);

#endif
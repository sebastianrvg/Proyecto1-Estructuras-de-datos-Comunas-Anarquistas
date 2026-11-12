#include <stdio.h>
#include <string.h>
#include "Busquedatexto.h"
#include "Colores.h"

struct NombreLibro *leerNombresLibros(char nombreArchivo[]) {
    /*
    Funcionamiento: abre el archivo que lista los libros de la biblioteca (una ruta por linea) y arma la lista enlazada simple con esos nombres. Ignora las lineas vacias
    Entradas: nombreArchivo (archivo con la lista de libros)
    Salidas: puntero al inicio de la lista de libros (NULL si no se pudo abrir o esta vacio)
    */
    FILE *archivo = NULL;
    struct NombreLibro *lista = NULL;
    char linea[100];
    int longitud = 0;

    archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return NULL;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        longitud = strlen(linea);
        // Quitar el salto de linea (y el \r de los archivos hechos en Windows)
        while (longitud > 0 && (linea[longitud - 1] == '\n' || linea[longitud - 1] == '\r')) {
            linea[longitud - 1] = '\0';
            longitud = longitud - 1;
        }
        if (longitud > 0) {
            lista = agregarNombreLibro(lista, linea);
        }
    }

    fclose(archivo);
    return lista;
}

int libroContieneTexto(char nombreLibro[], char texto[]) {
    /*
    Funcionamiento: lee el libro linea por linea y revisa si alguna contiene el texto (distingue mayusculas y minusculas, y el texto tiene que estar dentro de una misma linea)
    Entradas: nombreLibro (ruta del archivo), texto (texto que se busca)
    Salidas: 1 si el libro contiene el texto, 0 si no o si no se pudo abrir
    */
    FILE *archivo = NULL;
    char linea[4096]; // Pedazo de linea que se lee; una linea mas larga se lee en varios pedazos

    archivo = fopen(nombreLibro, "r");
    if (archivo == NULL) {
        printf(COLOR_AMARILLO "No se pudo abrir el libro %s\n" COLOR_RESET, nombreLibro);
        return 0;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, texto) != NULL) {
            fclose(archivo);
            return 1;
        }
    }

    fclose(archivo);
    return 0;
}

struct NombreLibro *buscarEnLibros(struct NombreLibro *libros, char texto[]) {
    /*
    Funcionamiento: revisa cada libro de la lista y arma una lista nueva con los que contienen el texto
    Entradas: libros (lista de libros de la biblioteca), texto (texto que se busca)
    Salidas: lista nueva con los nombres de los libros que contienen el texto (NULL si ninguno)
    */
    struct NombreLibro *actual = libros;
    struct NombreLibro *encontrados = NULL;

    while (actual != NULL) {
        if (libroContieneTexto(actual->nombre, texto) == 1) {
            encontrados = agregarNombreLibro(encontrados, actual->nombre);
        }
        actual = actual->siguiente;
    }

    return encontrados;
}

struct NodoChaflaneado *buscarTextoConCache(struct NodoChaflaneado *cache, struct NombreLibro *libros, char texto[], struct NombreLibro **resultado) {
    /*
    Funcionamiento: busca el texto primero en el arbol chaflaneado (cache). Si esta, usa el resultado guardado. Si no, lo busca en los archivos y guarda el resultado en el arbol. En los dos casos el texto queda en la raiz del arbol
    Entradas: cache (raiz del arbol chaflaneado), libros (lista de libros de la biblioteca), texto (texto que se busca),
              resultado (se llena con la lista de libros que contienen el texto; la lista es del arbol, no se debe liberar aparte)
    Salidas: nueva raiz del arbol chaflaneado
    */
    struct NombreLibro *encontrados = NULL;
    int encontrado = 0;

    printf("Buscando \"%s\" en el cache (arbol chaflaneado)\n", texto);
    cache = buscarTexto(cache, texto, &encontrado);

    if (encontrado == 1) {
        printf(COLOR_VERDE "Estaba en el cache: no hizo falta leer los libros\n" COLOR_RESET);
        *resultado = cache->libros;
        return cache;
    }

    printf("No estaba en el cache: buscando en los libros\n");
    encontrados = buscarEnLibros(libros, texto);
    cache = insertarTexto(cache, texto, encontrados);
    printf("Resultado guardado en el cache\n");

    *resultado = cache->libros;
    return cache;
}

void mostrarNombresLibro(struct NombreLibro *lista) {
    /*
    Funcionamiento: imprime los nombres de la lista, uno por linea, o un aviso si esta vacia
    Entradas: lista (inicio de la lista)
    Salidas: ninguna (imprime en pantalla)
    */
    if (lista == NULL) {
        printf("Ningun libro contiene ese texto\n");
        return;
    }

    printf("Libros que contienen el texto:\n");
    while (lista != NULL) {
        printf("  %s\n", lista->nombre);
        lista = lista->siguiente;
    }
}
#ifndef BIEN_H
#define BIEN_H

/*
Funcionamiento: representa un nodo de bien dentro de una comuna, con su
existencia actual, su maximo posible y un indicador de valor comunitario
(no es dinero ni precio).
Entradas: ninguna (es una definicion de tipo)
Salidas: ninguna (es una definicion de tipo)
*/
struct Bien {
    char nombre[50];
    int existencia;
    int maximo;
    float valor;
    struct Bien *siguiente;
};

/*
Funcionamiento: reserva memoria para un bien nuevo y le carga sus datos
iniciales.
Entradas: nombre (texto con el nombre del bien), existencia (cantidad
inicial), maximo (tope que puede alcanzar), valor (indicador comunitario
inicial)
Salidas: puntero al bien creado
*/
struct Bien *crearBien(char nombre[], int existencia, int maximo, float valor);

/*
Funcionamiento: inserta un bien al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (bien a insertar)
Salidas: lista actualizada (la misma, o el bien nuevo como inicio si
estaba vacia)
*/
struct Bien *insertarBien(struct Bien *lista, struct Bien *nueva);

/*
Funcionamiento: recorre la lista de bienes y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de bienes en la lista
*/
int recorrerBienes(struct Bien *lista);

/*
Funcionamiento: recorre la lista de bienes e imprime los datos de cada uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarBienes(struct Bien *lista);

/*
Funcionamiento: recorre la lista de bienes y libera la memoria de cada
nodo, uno por uno, para no dejar fugas de memoria.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarBienes(struct Bien *lista);

/*
Funcionamiento: abre el archivo de bienes, lee un nombre por linea y
construye la lista enlazada simple del catalogo base. La existencia, el
maximo y el valor quedan en 0 porque todavia no se sabe a que comuna van
a pertenecer ni cuantas personas tiene esa comuna.
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de bienes cargada
*/
struct Bien *leerBienes(char nombreArchivo[]);

#endif
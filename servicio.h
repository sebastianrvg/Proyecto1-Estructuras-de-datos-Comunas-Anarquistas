#ifndef SERVICIO_H
#define SERVICIO_H

/*
Funcionamiento: representa un nodo de servicio dentro de una comuna, con
su existencia actual, su maximo posible y un indicador de valor
comunitario (no es dinero ni precio).
Entradas: ninguna (es una definicion de tipo)
Salidas: ninguna (es una definicion de tipo)
*/
struct Servicio {
    char nombre[50];
    int existencia;
    int maximo;
    float valor;
    struct Servicio *siguiente;
};

/*
Funcionamiento: reserva memoria para un servicio nuevo y le carga sus
datos iniciales.
Entradas: nombre (texto con el nombre del servicio), existencia (cantidad
inicial), maximo (tope que puede alcanzar), valor (indicador comunitario
inicial)
Salidas: puntero al servicio creado
*/
struct Servicio *crearServicio(char nombre[], int existencia, int maximo, float valor);

/*
Funcionamiento: inserta un servicio al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (servicio a insertar)
Salidas: lista actualizada (la misma, o el servicio nuevo como inicio si
estaba vacia)
*/
struct Servicio *insertarServicio(struct Servicio *lista, struct Servicio *nueva);

/*
Funcionamiento: recorre la lista de servicios y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de servicios en la lista
*/
int recorrerServicios(struct Servicio *lista);

/*
Funcionamiento: recorre la lista de servicios e imprime los datos de cada
uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarServicios(struct Servicio *lista);

/*
Funcionamiento: recorre la lista de servicios y libera la memoria de cada
nodo, uno por uno, para no dejar fugas de memoria.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarServicios(struct Servicio *lista);

/*
Funcionamiento: abre el archivo de servicios, lee un nombre por linea y
construye la lista enlazada simple del catalogo base. La existencia, el
maximo y el valor quedan en 0 porque todavia no se sabe a que comuna van
a pertenecer ni cuantas personas tiene esa comuna.
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de servicios cargada
*/
struct Servicio *leerServicios(char nombreArchivo[]);

#endif
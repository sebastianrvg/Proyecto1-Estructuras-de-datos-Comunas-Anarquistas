#ifndef SOCIEDAD_H
#define SOCIEDAD_H

#include "Nodos.h"

/*
Cantidad maxima de cada bien o servicio segun las personas de la comuna.
Los bienes tienen un factor alto porque se consumen cada turno (cada persona
consume entre 1 y 5 por turno, 3 en promedio, asi que 60 alcanzan para unos
20 turnos). Los servicios no se consumen, solo bajan por emergencias, por eso
su factor es bajo.
*/
#define FACTOR_MAXIMO_BIENES 60
#define FACTOR_MAXIMO_SERVICIOS 2

/*
Al crear la sociedad, cada bien o servicio empieza con una cantidad al azar
entre este porcentaje de su maximo y el maximo completo.
*/
#define PORCENTAJE_INICIAL_MINIMO 60

// PEDIR DATOS

/*
Funcionamiento: le pide un numero entero al usuario y repite la pregunta hasta que escriba uno dentro del rango. Si se acaba la entrada de datos devuelve el minimo para no quedarse en un ciclo infinito.
Entradas: mensaje (texto de lo que se pregunta), minimo (valor minimo permitido), maximo (valor maximo permitido)
Salidas: el numero que escribio el usuario, dentro del rango
*/
int pedirCantidad(char mensaje[], int minimo, int maximo);

// ELEGIR

/*
Funcionamiento: elige al azar una cantidad exacta de comunas de la lista de comunas disponibles y las copia (solo el nombre) en una lista circular nueva. Cada comuna disponible tiene la misma probabilidad de ser elegida.
Entradas: disponibles (lista circular con todas las comunas del archivo), cantidadDisponibles (cuantas hay en esa lista), cantidadElegir (cuantas se quieren elegir)
Salidas: puntero al inicio de la lista circular con las comunas elegidas
*/
struct Comuna *elegirComunas(struct Comuna *disponibles, int cantidadDisponibles, int cantidadElegir);

/*
Funcionamiento: se queda solo con los primeros nodos de la lista de recursos y libera el resto.
Entradas: lista (inicio de la lista), cantidad (cuantos recursos se conservan, minimo 1)
Salidas: puntero al inicio de la lista recortada
*/
struct Recursos *recortarRecursos(struct Recursos *lista, int cantidad);

/*
Funcionamiento: se queda solo con los primeros nodos de la lista de servicios y libera el resto.
Entradas: lista (inicio de la lista), cantidad (cuantos servicios se conservan, minimo 1)
Salidas: puntero al inicio de la lista recortada
*/
struct Servicios *recortarServicios(struct Servicios *lista, int cantidad);

// REPARTIR Y ASIGNAR 

/*
Funcionamiento: reparte todas las personas entre las comunas, pasando cada nodo de la lista general a la lista de su comuna. Las cantidades son distintas entre comunas (cada una recibe cerca del promedio de lo que queda, mas o menos la mitad) y todas reciben al menos una persona. La ultima comuna recibe las que sobren. Al terminar, la lista general queda vacia y su puntero ya no se debe usar.
Entradas: inicio (lista circular de comunas), cantidadComunas (cuantas comunas hay), personas (lista general de personas)
Salidas: ninguna (cada comuna queda con su lista de personas)
*/
void repartirPersonas(struct Comuna *inicio, int cantidadComunas, struct Persona *personas);

/*
Funcionamiento: le da a una comuna su propia copia de cada recurso del catalogo. El maximo se calcula segun las personas de la comuna y la cantidad inicial es al azar entre el porcentaje inicial minimo y el maximo.
Entradas: comuna (comuna que recibe los recursos), catalogo (lista base de recursos)
Salidas: ninguna (la comuna queda con su lista de recursos)
*/
void asignarRecursos(struct Comuna *comuna, struct Recursos *catalogo);

/*
Funcionamiento: le da a una comuna su propia copia de cada servicio del catalogo, conservando el oficio de cada uno. El maximo se calcula segun las personas de la comuna y la cantidad inicial es al azar entre el porcentaje inicial minimo y el maximo.
Entradas: comuna (comuna que recibe los servicios), catalogo (lista base de servicios)
Salidas: ninguna (la comuna queda con su lista de servicios)
*/
void asignarServicios(struct Comuna *comuna, struct Servicios *catalogo);

// CREAR LA SOCIEDAD

/*
Funcionamiento: crea la sociedad completa. Lee los archivos personas, bienes, servicios y comunas, le pregunta al usuario cuantas comunas, bienes y servicios quiere, reparte las personas, y le da a cada comuna sus bienes y servicios con sus maximos y cantidades iniciales.
Entradas: ninguna (los datos los pide al usuario)
Salidas: puntero al inicio de la lista circular de comunas, o NULL si no se pudo crear (por ejemplo, si falta un archivo)
*/
struct Comuna *crearSociedad(void);

// LIBERAR

/*
Funcionamiento: recorre la lista de personas y libera la memoria de cada nodo, uno por uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarPersonas(struct Persona *lista);

/*
Funcionamiento: libera toda la lista circular de comunas, incluyendo las listas de personas, recursos y servicios de cada una.
Entradas: inicio (inicio de la lista circular)
Salidas: ninguna (libera memoria)
*/
void liberarComunas(struct Comuna *inicio);

#endif
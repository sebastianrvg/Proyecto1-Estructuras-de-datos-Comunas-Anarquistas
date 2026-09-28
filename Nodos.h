#ifndef NODOS_H
#define NODOS_H

struct Recursos {
    /*
    Funcionamiento: Sirve para representar los recursos disponibles en la comuna, como alimentos, medicinas, herramientas, etc. 
    Entrada: Cada recurso tiene un nombre, una cantidad actual y una cantidad máxima que puede ser almacenada. 
    Además, se utiliza un puntero al siguiente recurso para formar una lista enlazada simple de recursos.
    */
    char nombre[50]; // Nombre del recurso
    int cantidad; // Cantidad actual del recurso
    int cantidadMaxima; // Cantidad máxima que puede ser almacenada
    struct Recursos *siguiente; 
};

struct Servicios {
    /*
    Funcionamiento: Sirve para representa los servicios disponibles en la comuna, como educación, salud, transporte, etc.
    Entrada: Cada servicio tiene un nombre, una cantidad actual y una cantidad máxima que puede ser ofrecida
    Además, se utiliza un puntero al siguiente servicio para formar una lista enlazada simple de servicios.
    */
    char nombre[50]; // Nombre del servicio
    int cantidad; // Cantidad actual del servicio
    int cantidadMaxima; // Cantidad máxima que puede ser ofrecida
    struct Servicios *siguiente;
};

/*
Funcionamiento: define la estructura de un nodo de persona para la lista
enlazada simple de la sociedad.
Entradas: ninguna (es una definición de tipo)
Salidas: ninguna (es una definición de tipo)
*/
struct Persona {
    char nombre[50];
    struct Persona *siguiente;
};

struct Comuna {
    /*
    Entrada: Cada comuna tiene un nombre, una lista de personas que la conforman, una lista de recursos disponibles, una lista de servicios disponibles,
    un nivel de necesidad y un nivel de satisfacción.
    Además, se utiliza un puntero a la siguiente comuna para formar una lista enlazada circular de comunas.
    */
    char nombre[50];
    struct Persona *personas; // Lista de personas en la comuna
    struct Recursos *bienes; // Lista de recursos disponibles en la comuna
    struct Servicios *servicios; // Lista de servicios disponibles en la comuna
    float necesidad; // Nivel de necesidad de la comuna
    float satisfaccion; // Nivel de satisfacción de la comuna
    struct Comuna *siguiente;
};

/*
Funcionamiento: reserva memoria para una nueva persona y copia su nombre.
Entradas: nombre (texto con el nombre de la persona)
Salidas: puntero a la persona creada
*/
struct Persona *crearPersona(char nombre[]);
 
/*
Funcionamiento: inserta una persona al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (persona a insertar)
Salidas: lista actualizada (la misma, o la nueva persona como inicio si estaba vacía)
*/
struct Persona *insertarPersona(struct Persona *lista, struct Persona *nueva);
 
/*
Funcionamiento: recorre la lista de personas y cuenta cuántas hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de personas en la lista
*/
int recorrerPersonas(struct Persona *lista);
 
/*
Funcionamiento: recorre la lista de personas e imprime el nombre de cada una.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarPersonas(struct Persona *lista);

/*
Funcionamiento: reserva memoria para un nuevo recurso y copia sus datos.
Entradas: nombre (texto con el nombre del recurso), cantidad (cantidad actual),
cantidadMaxima (cantidad máxima que puede ser almacenada)
Salidas: puntero al recurso creado
*/
struct Recursos *crearRecurso(char nombre[], int cantidad, int cantidadMaxima);

/*
Funcionamiento: reserva memoria para un nuevo servicio y copia sus datos.
Entradas: nombre (texto con el nombre del servicio), cantidad (cantidad actual)
Salidas: puntero al servicio creado
*/
struct Servicios *crearServicio(char nombre[], int cantidad, int cantidadMaxima);

struct Comuna *crearNodoCircular(struct Comuna *lista, char nombre[]);
struct Comuna *avanzarComunas(struct Comuna *actual, int pasos, int cantidadComunas);

#endif
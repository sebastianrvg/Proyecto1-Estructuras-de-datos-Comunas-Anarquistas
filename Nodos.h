#ifndef NODOS_H
#define NODOS_H

struct Recursos {
    /*
    Sirve para representar los recursos disponibles en la comuna, como alimentos, medicinas, herramientas, etc. 
    Cada recurso tiene un nombre, una cantidad actual y una cantidad máxima que puede ser almacenada. 
    Además, se utiliza un puntero al siguiente recurso para formar una lista enlazada simple de recursos.
    */
    char nombre[50]; // Nombre del recurso
    int cantidad; // Cantidad actual del recurso
    int cantidadMaxima; // Cantidad máxima que puede ser almacenada
    struct Recursos *siguiente; 
};

struct Servicios {
    /*
    Representa los servicios disponibles en la comuna, como educación, salud, transporte, etc.
    Cada servicio tiene un nombre, una cantidad actual y una cantidad máxima que puede ser ofrecida
    Además, se utiliza un puntero al siguiente servicio para formar una lista enlazada simple de servicios.
    */
    char nombre[50]; // Nombre del servicio
    int cantidad; // Cantidad actual del servicio
    int cantidadMaxima; // Cantidad máxima que puede ser ofrecida
    struct Servicios *siguiente;
};

struct Comuna {
    /*
    Representa las comuna.
    Cada comuna tiene un nombre, una lista de personas que la conforman, una lista de recursos disponibles, una lista de servicios disponibles,
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

void crearRecurso(); 
void crearServicio();
void crearNodoCircular();

#endif
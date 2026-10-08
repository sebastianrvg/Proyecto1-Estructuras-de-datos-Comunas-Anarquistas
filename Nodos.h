#ifndef NODOS_H
#define NODOS_H

/*
Consumo de bienes por persona en cada turno. Todas las funciones que aplican
o estiman consumo usan estas mismas constantes, para que el modelo sea el mismo
en toda la simulacion. El promedio es el punto medio entre el minimo y el maximo.
*/
#define CONSUMO_MINIMO_POR_PERSONA 1
#define CONSUMO_MAXIMO_POR_PERSONA 5
#define CONSUMO_PROMEDIO_POR_PERSONA 3

/*
Cada punto de gravedad de una emergencia quita este porcentaje de la cantidad
maxima del recurso o servicio afectado (con un minimo de 1 unidad).
*/
#define PORCENTAJE_PERDIDA_POR_PUNTO 5

/*
Porcentaje de la cantidad intercambiada que cada comuna recibe de mas en un
trueque (bono de reciprocidad). Es lo que hace subir el valor de los bienes
y servicios cuando las comunas cooperan.
*/
#define PORCENTAJE_BONO_TRUEQUE 30


/*
Regalo despues de una emergencia: probabilidad (en porcentaje) de que otra comuna
le regale recursos a la comuna afectada, y rango de la cantidad que se regala.
*/
#define PROBABILIDAD_REGALO 25
#define REGALO_MINIMO 5
#define REGALO_MAXIMO 10

struct Recursos {
    /*
    Funcionamiento: Sirve para representar los recursos disponibles en la comuna, como alimentos, medicinas, herramientas, etc. 
    Entrada: Cada recurso tiene un nombre, una cantidad actual y una cantidad máxima que puede ser almacenada. 
    Además, se utiliza un puntero al siguiente recurso para formar una lista enlazada simple de recursos.
    */
    char nombre[50]; // Nombre del recurso
    int cantidad; // Cantidad actual del recurso
    int cantidadMaxima; // Cantidad máxima que puede ser almacenada
    int emergencias; // 1 si el recurso esta en emergencia (no alcanza para 2 turnos de consumo), 0 si no
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
    int emergencias; // 1 si el servicio esta en emergencia (menos del 20% del reparto esperado), 0 si no
    int oficio; // Numero que identifica el servicio, es el mismo que usa Persona (posicion en el archivo, empieza en 1)
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
    int oficio; // Numero del servicio al que pertenece la persona (0 si todavia no tiene)
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

// ==================== CREAR ====================

/*
Funcionamiento: reserva memoria para una nueva persona y copia su nombre.
Entradas: nombre (texto con el nombre de la persona)
Salidas: puntero a la persona creada
*/
struct Persona *crearPersona(char nombre[]);

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

// ==================== RECORRER ====================

/*
Funcionamiento: recorre la lista de personas y cuenta cuántas hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de personas en la lista
*/
int recorrerPersonas(struct Persona *lista);

/*
Funcionamiento: recorre la lista de recursos y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de recursos en la lista
*/
int recorrerRecursos(struct Recursos *lista);

/*
Funcionamiento: recorre la lista de servicios y cuenta cuantos hay.
Entradas: lista (inicio de la lista)
Salidas: cantidad de servicios en la lista
*/
int recorrerServicios(struct Servicios *lista);

/*
Funcionamiento: recorre la lista circular de comunas y cuenta cuantas hay.
Entradas: lista (inicio de la lista circular)
Salidas: cantidad de comunas en la lista
*/
int recorrerComunas(struct Comuna *lista);

struct Comuna *avanzarComunas(struct Comuna *actual, int pasos, int cantidadComunas);

// ==================== MOSTRAR ==================== 

/*
Funcionamiento: recorre la lista de personas e imprime el nombre de cada una.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarPersonas(struct Persona *lista);

/*
Funcionamiento: recorre la lista de recursos e imprime nombre, cantidad, cantidadMaxima y emergencias de cada uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarRecursos(struct Recursos *lista);

/*
Funcionamiento: recorre la lista de servicios e imprime nombre, cantidad, cantidadMaxima, emergencias y oficio de cada uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarServicios(struct Servicios *lista);

/*
Funcionamiento: recorre la lista circular de comunas e imprime nombre, necesidad y satisfaccion de cada una.
Entradas: lista (inicio de la lista circular)
Salidas: ninguna (imprime en pantalla)
*/
void mostrarComunas(struct Comuna *lista);

// ==================== INSERTAR Y LEER ==================== 

/*
Funcionamiento: inserta una persona al final de la lista enlazada simple.
Entradas: lista (inicio actual de la lista), nueva (persona a insertar)
Salidas: lista actualizada (la misma, o la nueva persona como inicio si estaba vacía)
*/
struct Persona *insertarPersona(struct Persona *lista, struct Persona *nueva);

/*
Funcionamiento: inserta un recurso al final de la lista enlazada simple de recursos.
Entradas: lista (inicio actual de la lista), nueva (recurso a insertar)
Salidas: lista actualizada (la misma, o el recurso nuevo como inicio si estaba vacía)
*/
struct Recursos *insertarRecurso(struct Recursos *lista, struct Recursos *nueva);

/*
Funcionamiento: inserta un servicio al final de la lista enlazada simple de servicios.
Entradas: lista (inicio actual de la lista), nueva (servicio a insertar)
Salidas: lista actualizada (la misma, o el servicio nuevo como inicio si estaba vacía)
*/
struct Servicios *insertarServicio(struct Servicios *lista, struct Servicios *nueva);

/*
Funcionamiento: abre el archivo de personas, lee un nombre por linea y
construye la lista enlazada simple completa. A cada persona le asigna un
oficio al azar entre 1 y cantidadOficios.
Entradas: nombreArchivo (ruta o nombre del archivo a leer),
cantidadOficios (cantidad de servicios activos en la simulacion)
Salidas: puntero al inicio de la lista de personas cargada
*/
struct Persona *leerPersonas(char nombreArchivo[], int cantidadOficios);

/*
Funcionamiento: abre el archivo de bienes y construye la lista de recursos del catalogo base (cantidad y cantidadMaxima en 0).
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de recursos cargada
*/
struct Recursos *leerRecursos(char nombreArchivo[]);

/*
Funcionamiento: abre el archivo de servicios y construye la lista del catalogo base (cantidad y cantidadMaxima en 0).
Ignora las lineas vacias y le da a cada servicio su numero de oficio segun su posicion en el archivo (empieza en 1).
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista de servicios cargada
*/
struct Servicios *leerServicios(char nombreArchivo[]);

/*
Funcionamiento: abre el archivo de comunas y construye la lista circular usando crearNodoCircular.
Entradas: nombreArchivo (ruta o nombre del archivo a leer)
Salidas: puntero al inicio de la lista circular de comunas cargada
*/
struct Comuna *leerComunas(char nombreArchivo[]);

/* ==================== LIBERAR ==================== */

/*
Funcionamiento: recorre la lista de recursos y libera la memoria de cada nodo, uno por uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarRecursos(struct Recursos *lista);

/*
Funcionamiento: recorre la lista de servicios y libera la memoria de cada nodo, uno por uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarServicios(struct Servicios *lista);

#endif

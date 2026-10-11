#include <stdio.h>
#include <stdlib.h>
#include "Sociedad.h"
#include "Colores.h"

// ==================== PEDIR DATOS ====================

/*
Funcionamiento: le pide un numero entero al usuario y repite la pregunta hasta que escriba uno dentro del rango. Si se acaba la entrada de datos devuelve el minimo para no quedarse en un ciclo infinito.
Entradas: mensaje (texto de lo que se pregunta), minimo (valor minimo permitido), maximo (valor maximo permitido)
Salidas: el numero que escribio el usuario, dentro del rango
*/
int pedirCantidad(char mensaje[], int minimo, int maximo) {
    char error[100]; // Para descartar lo que escriba el usuario si no es un numero
    int numero = 0; // Numero que escribio el usuario
    int resultado = 0; // Resultado de scanf: 1 si leyo un numero, otro valor si no
    int valido = 0; // 1 cuando el usuario ya escribio un numero dentro del rango

    while (valido == 0) {
        printf("%s (%d a %d): ", mensaje, minimo, maximo);
        resultado = scanf("%d", &numero);

        if (resultado == EOF) {
            return minimo;
        }

        if (resultado != 1) {
            scanf("%99s", error);
            printf("Opcion invalida.\n");
        } else if (numero < minimo || numero > maximo) {
            printf("Opcion invalida.\n");
        } else {
            valido = 1;
        }
    }

    return numero;
}

// ==================== ELEGIR ====================

/*
Funcionamiento: elige al azar una cantidad exacta de comunas de la lista de comunas disponibles y las copia (solo el nombre) en una lista circular nueva. Cada comuna disponible tiene la misma probabilidad de ser elegida.
Entradas: disponibles (lista circular con todas las comunas del archivo), cantidadDisponibles (cuantas hay en esa lista), cantidadElegir (cuantas se quieren elegir)
Salidas: puntero al inicio de la lista circular con las comunas elegidas
*/
struct Comuna *elegirComunas(struct Comuna *disponibles, int cantidadDisponibles, int cantidadElegir) {
    struct Comuna *actual = disponibles;
    struct Comuna *elegidas = NULL;
    int yaElegidas = 0; // Cuantas comunas se han elegido hasta ahora
    int quedanPorRevisar = 0; // Cuantas comunas faltan por revisar, contando la actual

    // Se revisa cada comuna una sola vez. Se elige con probabilidad (faltan por elegir / quedan por revisar), asi siempre se llega a la cantidad exacta y todas tienen la misma probabilidad
    for (int i = 0; i < cantidadDisponibles && yaElegidas < cantidadElegir; i++) {
        quedanPorRevisar = cantidadDisponibles - i;
        if (rand() % quedanPorRevisar < cantidadElegir - yaElegidas) {
            elegidas = crearNodoCircular(elegidas, actual->nombre);
            yaElegidas = yaElegidas + 1;
        }
        actual = actual->siguiente;
    }

    return elegidas;
}

/*
Funcionamiento: se queda solo con los primeros nodos de la lista de recursos y libera el resto.
Entradas: lista (inicio de la lista), cantidad (cuantos recursos se conservan, minimo 1)
Salidas: puntero al inicio de la lista recortada
*/
struct Recursos *recortarRecursos(struct Recursos *lista, int cantidad) {
    struct Recursos *actual = lista;
    struct Recursos *sobrante = NULL;
    int posicion = 1; // Posicion del nodo actual dentro de la lista

    while (actual != NULL && posicion < cantidad) {
        actual = actual->siguiente;
        posicion = posicion + 1;
    }

    // Cortar la lista despues del ultimo nodo que se conserva y liberar lo que sobra
    if (actual != NULL) {
        sobrante = actual->siguiente;
        actual->siguiente = NULL;
        liberarRecursos(sobrante);
    }

    return lista;
}

/*
Funcionamiento: se queda solo con los primeros nodos de la lista de servicios y libera el resto.
Entradas: lista (inicio de la lista), cantidad (cuantos servicios se conservan, minimo 1)
Salidas: puntero al inicio de la lista recortada
*/
struct Servicios *recortarServicios(struct Servicios *lista, int cantidad) {
    struct Servicios *actual = lista;
    struct Servicios *sobrante = NULL;
    int posicion = 1; // Posicion del nodo actual dentro de la lista

    while (actual != NULL && posicion < cantidad) {
        actual = actual->siguiente;
        posicion = posicion + 1;
    }

    // Cortar la lista despues del ultimo nodo que se conserva y liberar lo que sobra
    if (actual != NULL) {
        sobrante = actual->siguiente;
        actual->siguiente = NULL;
        liberarServicios(sobrante);
    }

    return lista;
}

// ==================== REPARTIR Y ASIGNAR ====================

/*
Funcionamiento: reparte todas las personas entre las comunas, pasando cada nodo de la lista general a la lista de su comuna. Las cantidades son distintas entre comunas (cada una recibe cerca del promedio de lo que queda, mas o menos la mitad) y todas reciben al menos una persona. La ultima comuna recibe las que sobren. Al terminar, la lista general queda vacia y su puntero ya no se debe usar.
Entradas: inicio (lista circular de comunas), cantidadComunas (cuantas comunas hay), personas (lista general de personas)
Salidas: ninguna (cada comuna queda con su lista de personas)
*/
void repartirPersonas(struct Comuna *inicio, int cantidadComunas, struct Persona *personas) {
    struct Comuna *comuna = inicio;
    struct Persona *nodo = NULL; // Persona que se esta pasando a la comuna
    int restantes = recorrerPersonas(personas); // Personas que todavia no tienen comuna
    int comunasRestantes = 0; // Comunas que todavia no reciben personas, contando la actual
    int promedio = 0; // Promedio de personas que le tocarian a cada comuna restante
    int cantidad = 0; // Personas que recibe la comuna actual
    int maximoPermitido = 0; // Lo maximo que se le puede dar a esta comuna sin dejar a las demas sin nadie

    for (int i = 0; i < cantidadComunas; i++) {
        comunasRestantes = cantidadComunas - i;

        if (comunasRestantes == 1) {
            // La ultima comuna recibe todas las que sobran
            cantidad = restantes;
        } else {
            promedio = restantes / comunasRestantes;
            cantidad = promedio / 2 + rand() % (promedio + 1);
            maximoPermitido = restantes - (comunasRestantes - 1);

            if (cantidad < 1) {
                cantidad = 1;
            }
            if (cantidad > maximoPermitido) {
                cantidad = maximoPermitido;
            }
        }

        // Pasar las personas de la lista general a la lista de la comuna
        for (int j = 0; j < cantidad; j++) {
            nodo = personas;
            personas = personas->siguiente;
            nodo->siguiente = NULL;
            comuna->personas = insertarPersona(comuna->personas, nodo);
        }

        restantes = restantes - cantidad;
        comuna = comuna->siguiente;
    }
}

/*
Funcionamiento: le da a una comuna su propia copia de cada recurso del catalogo. El maximo se calcula segun las personas de la comuna y la cantidad inicial es al azar entre el porcentaje inicial minimo y el maximo.
Entradas: comuna (comuna que recibe los recursos), catalogo (lista base de recursos)
Salidas: ninguna (la comuna queda con su lista de recursos)
*/
void asignarRecursos(struct Comuna *comuna, struct Recursos *catalogo) {
    struct Recursos *actual = catalogo;
    struct Recursos *nuevo = NULL;
    int personas = recorrerPersonas(comuna->personas); // Personas de la comuna
    int maximo = 0; // Cantidad maxima del recurso en esta comuna
    int minimo = 0; // Cantidad inicial mas baja posible
    int cantidad = 0; // Cantidad inicial que se le da al recurso

    while (actual != NULL) {
        maximo = personas * FACTOR_MAXIMO_BIENES;
        minimo = maximo * PORCENTAJE_INICIAL_MINIMO / 100;
        cantidad = minimo + rand() % (maximo - minimo + 1);

        nuevo = crearRecurso(actual->nombre, cantidad, maximo);
        comuna->bienes = insertarRecurso(comuna->bienes, nuevo);

        actual = actual->siguiente;
    }
}

/*
Funcionamiento: le da a una comuna su propia copia de cada servicio del catalogo, conservando el oficio de cada uno. El maximo se calcula segun las personas de la comuna y la cantidad inicial es al azar entre el porcentaje inicial minimo y el maximo.
Entradas: comuna (comuna que recibe los servicios), catalogo (lista base de servicios)
Salidas: ninguna (la comuna queda con su lista de servicios)
*/
void asignarServicios(struct Comuna *comuna, struct Servicios *catalogo) {
    struct Servicios *actual = catalogo;
    struct Servicios *nuevo = NULL;
    int personas = recorrerPersonas(comuna->personas); // Personas de la comuna
    int maximo = 0; // Cantidad maxima del servicio en esta comuna
    int minimo = 0; // Cantidad inicial mas baja posible
    int cantidad = 0; // Cantidad inicial que se le da al servicio

    while (actual != NULL) {
        maximo = personas * FACTOR_MAXIMO_SERVICIOS;
        minimo = maximo * PORCENTAJE_INICIAL_MINIMO / 100;
        cantidad = minimo + rand() % (maximo - minimo + 1);

        nuevo = crearServicio(actual->nombre, cantidad, maximo);
        nuevo->oficio = actual->oficio;
        comuna->servicios = insertarServicio(comuna->servicios, nuevo);

        actual = actual->siguiente;
    }
}

// ==================== CREAR LA SOCIEDAD ====================

/*
Funcionamiento: crea la sociedad completa. Lee los archivos personas, bienes, servicios y comunas, le pregunta al usuario cuantas comunas, bienes y servicios quiere, reparte las personas, y le da a cada comuna sus bienes y servicios con sus maximos y cantidades iniciales.
Entradas: ninguna (los datos los pide al usuario)
Salidas: puntero al inicio de la lista circular de comunas, o NULL si no se pudo crear (por ejemplo, si falta un archivo)
*/
struct Comuna *crearSociedad(void) {
    struct Comuna *disponibles = NULL; // Todas las comunas del archivo
    struct Comuna *sociedad = NULL; // Solo las comunas elegidas
    struct Comuna *comuna = NULL;
    struct Recursos *bienes = NULL; // Catalogo de bienes
    struct Servicios *servicios = NULL; // Catalogo de servicios
    struct Persona *personas = NULL; // Lista general de personas
    int totalComunas = 0; // Comunas que hay en el archivo
    int totalBienes = 0; // Bienes que hay en el archivo
    int totalServicios = 0; // Servicios que hay en el archivo
    int totalPersonas = 0; // Personas que hay en el archivo
    int cantidadComunas = 0; // Comunas que elige el usuario
    int cantidadBienes = 0; // Bienes que elige el usuario
    int cantidadServicios = 0; // Servicios que elige el usuario

    // Leer los archivos base de comunas, bienes y servicios
    disponibles = leerComunas("comunas");
    bienes = leerRecursos("bienes");
    servicios = leerServicios("servicios");

    totalComunas = recorrerComunas(disponibles);
    totalBienes = recorrerRecursos(bienes);
    totalServicios = recorrerServicios(servicios);

    // Para poder crear la sociedad hacen falta al menos 2 comunas, 1 bien y 1 servicio
    if (totalComunas < 2 || totalBienes < 1 || totalServicios < 1) {
        printf("Los archivos comunas, bienes y servicios no tienen suficientes datos para crear la sociedad.\n");
        liberarComunas(disponibles);
        liberarRecursos(bienes);
        liberarServicios(servicios);
        return NULL;
    }

    // Preguntarle al usuario las cantidades que va a usar la simulacion
    cantidadComunas = pedirCantidad("Cantidad de comunas", 2, totalComunas);
    cantidadBienes = pedirCantidad("Cantidad de bienes", 1, totalBienes);
    cantidadServicios = pedirCantidad("Cantidad de servicios", 1, totalServicios);
    printf("-----------------------------------------------------------------------------------------------------\n");

    // Quedarse solo con lo que eligio el usuario
    bienes = recortarRecursos(bienes, cantidadBienes);
    servicios = recortarServicios(servicios, cantidadServicios);
    sociedad = elegirComunas(disponibles, totalComunas, cantidadComunas);
    liberarComunas(disponibles);

    // Leer las personas (cada una recibe un oficio entre 1 y la cantidad de servicios elegidos)
    personas = leerPersonas("personas", cantidadServicios);
    totalPersonas = recorrerPersonas(personas);

    // Cada comuna necesita al menos una persona
    if (totalPersonas < cantidadComunas) {
        printf("El archivo personas no tiene suficientes personas para %d comunas.\n", cantidadComunas);
        liberarPersonas(personas);
        liberarComunas(sociedad);
        liberarRecursos(bienes);
        liberarServicios(servicios);
        return NULL;
    }

    // Repartir las personas y darle a cada comuna sus bienes y servicios
    repartirPersonas(sociedad, cantidadComunas, personas);
    personas = NULL;

    comuna = sociedad;
    for (int i = 0; i < cantidadComunas; i++) {
        asignarRecursos(comuna, bienes);
        asignarServicios(comuna, servicios);
        comuna = comuna->siguiente;
    }

    // Los catalogos ya no hacen falta, cada comuna tiene sus propias copias
    liberarRecursos(bienes);
    liberarServicios(servicios);

    printf(COLOR_VERDE "Sociedad creada: %d comunas, %d bienes, %d servicios y %d personas.\n" COLOR_RESET, cantidadComunas, cantidadBienes, cantidadServicios, totalPersonas);

    return sociedad;
}

// ==================== LIBERAR ====================

/*
Funcionamiento: recorre la lista de personas y libera la memoria de cada nodo, uno por uno.
Entradas: lista (inicio de la lista)
Salidas: ninguna (libera memoria)
*/
void liberarPersonas(struct Persona *lista) {
    struct Persona *actual = lista;
    struct Persona *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

/*
Funcionamiento: libera toda la lista circular de comunas, incluyendo las listas de personas, recursos y servicios de cada una.
Entradas: inicio (inicio de la lista circular)
Salidas: ninguna (libera memoria)
*/
void liberarComunas(struct Comuna *inicio) {
    struct Comuna *actual = inicio;
    struct Comuna *siguiente = NULL;
    int cantidad = recorrerComunas(inicio); // Se cuenta antes de liberar, porque en una lista circular no hay NULL que marque el final

    for (int i = 0; i < cantidad; i++) {
        siguiente = actual->siguiente;
        liberarPersonas(actual->personas);
        liberarRecursos(actual->bienes);
        liberarServicios(actual->servicios);
        free(actual);
        actual = siguiente;
    }
}

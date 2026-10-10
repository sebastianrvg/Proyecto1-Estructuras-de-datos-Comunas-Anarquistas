#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FuncionesAux_Gustavo.h"

void calcularPuntosRecursos(struct Recursos *lista, int personas, int *suma_puntos, int *cantidad_emergencias) {
    /*
    Funcionamiento: Calcula los puntos de satisfacción y la cantidad de emergencias para los recursos de una comuna
    Entradas: lista (puntero al inicio de la lista de recursos), 
        personas (cantidad de personas en la comuna), 
        suma_puntos (puntero a la variable donde se almacenará la suma de puntos),
        cantidad_emergencias (puntero a la variable donde se almacenará la cantidad de emergencias)
    Salidas: suma_puntos y cantidad_emergencias actualizados
    */
    struct Recursos *actual = lista;
    int estimado_1_turno = 0;
    int estimado_2_turnos = 0;
    int puntos = 0;
    float margen = 0;

    *suma_puntos = 0;
    *cantidad_emergencias = 0;

    // Contar la estimación de recursos necesarios para 2 turnos
    while (actual != NULL) {
        // Estimar la cantidad de recursos necesarios para 1 turno con el consumo promedio por persona
        // (sin azar, para que la misma situacion de siempre el mismo resultado) y luego multiplicar por 2
        estimado_1_turno = personas * CONSUMO_PROMEDIO_POR_PERSONA;
        estimado_2_turnos = estimado_1_turno * 2;

        // Calcular los puntos de satisfacción y emergencias según la cantidad disponible
        if (actual->cantidad < estimado_2_turnos) {
            puntos = -1;
            (*cantidad_emergencias)++;
        } else if (actual->cantidad >= (int)(0.9 * actual->cantidadMaxima)) {
            puntos = 2;
        } else {
            margen = (float)(actual->cantidad - estimado_2_turnos) / estimado_2_turnos;
            if (margen >= 1) {
                puntos = 1;
            } else {
                puntos = 0;
            }
        }

        if (puntos == -1) {
            actual->emergencias = 1;
        } else {
            actual->emergencias = 0;
        }

        // Actualizar la suma de puntos si no es una emergencia
        if (puntos != -1) {
            *suma_puntos = *suma_puntos + puntos;
        }

        actual = actual->siguiente;
    }
}

void calcularPuntosServicios(struct Servicios *lista, int personas, int *suma_puntos, int *cantidad_emergencias) {
    /*
    Funcionamiento: Calcula los puntos de satisfacción y la cantidad de emergencias para los servicios de una comuna
    Entradas: lista (puntero al inicio de la lista de servicios),
        personas (cantidad de personas en la comuna),
        suma_puntos (puntero a la variable donde se almacenará la suma de puntos),
        cantidad_emergencias (puntero a la variable donde se almacenará la cantidad de emergencias)
    Salidas: suma_puntos y cantidad_emergencias actualizados
    */
    struct Servicios *actual = lista;
    int num_tipos = 0;
    int puntos = 0;
    float reparto_esperado = 0;
    float ratio = 0;

    *suma_puntos = 0;
    *cantidad_emergencias = 0;

    // Contar el número de tipos de servicios disponibles
    while (actual != NULL) {
        num_tipos = num_tipos + 1;
        actual = actual->siguiente;
    }

    // Si no hay tipos de servicios, no se puede calcular la satisfacción
    if (num_tipos == 0) {
        return;
    }

    // Calcular el reparto esperado de servicios por persona
    reparto_esperado = (float)personas / num_tipos;

    actual = lista;

    // Calcular los puntos de satisfacción y emergencias según la cantidad disponible de cada servicio
    while (actual != NULL) {
        ratio = actual->cantidad / reparto_esperado;

        if (ratio < 0.2) { // Si la cantidad disponible es menos del 20% del reparto esperado, es una emergencia
            puntos = -1;
            (*cantidad_emergencias)++;
        } else if (ratio <= 0.6) { // Si la cantidad disponible es entre el 20% y el 60% del reparto esperado, se asigna 0 puntos
            puntos = 0;
        } else if (ratio < 1) { // Si la cantidad disponible es entre el 60% y el 100% del reparto esperado, se asigna 1 punto
            puntos = 1;
        } else { // Si la cantidad disponible es igual o mayor al reparto esperado, se asignan 2 puntos
            puntos = 2;
        }

        // Actualizar el estado de emergencia del servicio según los puntos calculadosd
        if (puntos == -1) {
            actual->emergencias = 1;
        } else {
            actual->emergencias = 0;
        }

        // Actualizar la suma de puntos si no es una emergencia
        if (puntos != -1) {
            *suma_puntos = *suma_puntos + puntos;
        }

        actual = actual->siguiente;
    }
}

void alcanceEmergencia(struct Comuna *comuna, int perdida, char *nombre_emergencia) {
    /*
    Funcionamiento: Aplica el efecto de una emergencia a un recurso o servicio aleatorio de la comuna
    Entradas: comuna (puntero a la comuna afectada), 
    perdida (gravedad de la emergencia; cada punto quita un porcentaje de la cantidad maxima del bien o servicio afectado), 
            nombre_emergencia (nombre de la emergencia)
    Salidas: ninguna (actualiza directamente los bienes o servicios de la comuna)
    */
    int bien_seleccionado = 0;
    int cantidadNodos = 0;
    int indice = 0;
    int perdida_real = 0; // Unidades que realmente se pierden segun la cantidad maxima del bien o servicio afectado

    // Seleccionar aleatoriamente si la emergencia afectará un recurso o un servicio
    bien_seleccionado = rand() % 2;

    // si se selecciona un recurso, contar la cantidad de recursos en la comuna
    if (bien_seleccionado == 1) {
        struct Recursos *actual = comuna->bienes;

        while (actual != NULL) {
            cantidadNodos = cantidadNodos + 1;
            actual = actual->siguiente;
        }

        if (cantidadNodos == 0) {
            return;
        }

        indice = rand() % cantidadNodos;
        actual = comuna->bienes;
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        // Calcular las unidades perdidas como porcentaje de la cantidad maxima (minimo 1 unidad)
        perdida_real = (actual->cantidadMaxima * perdida * PORCENTAJE_PERDIDA_POR_PUNTO) / 100;
        if (perdida_real < 1) {
            perdida_real = 1;
        }

        if (actual->emergencias == 1) {
            printf("Ha ocurrido %s en %s. El bien %s ya estaba en emergencia y perdio %d unidades mas.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        } else {
            printf("Ha ocurrido %s en %s. Se vio afectado el bien %s, se perdieron %d unidades.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        }

        actual->cantidad = actual->cantidad - perdida_real;
        if (actual->cantidad < 0) {
            actual->cantidad = 0;
        }

    // Si lo que salio fue un servicio
    } else {
        struct Servicios *actual = comuna->servicios; // Puntero para recorrer la lista de servicios

        // Contar la cantidad de servicios en la comuna
        while (actual != NULL) {
            cantidadNodos = cantidadNodos + 1;
            actual = actual->siguiente;
        }

        // Si no hay servicios, no se puede aplicar la emergencia
        if (cantidadNodos == 0) {
            return;
        }

        indice = rand() % cantidadNodos; // Seleccionar un índice aleatorio para el servicio afectado
        actual = comuna->servicios; // Puntero para recorrer la lista de servicios nuevamente

        // Avanzar hasta el servicio seleccionado
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        // Calcular las unidades perdidas como porcentaje de la cantidad maxima (minimo 1 unidad)
        perdida_real = (actual->cantidadMaxima * perdida * PORCENTAJE_PERDIDA_POR_PUNTO) / 100;
        if (perdida_real < 1) {
            perdida_real = 1;
        }

        // Si el servicio ya estaba en emergencia, informar que perdió más unidades
        if (actual->emergencias == 1) {
            printf("Ha ocurrido %s en %s. El servicio %s ya estaba en emergencia y perdio %d unidades mas.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        
        // Si el servicio no estaba en emergencia, informar que se vio afectado y perdió unidades
        } else {
            printf("Ha ocurrido %s en %s. Se vio afectado el servicio %s, se perdieron %d unidades.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        }

        // Actualizar la cantidad del servicio afectado por la emergencia
        actual->cantidad = actual->cantidad - perdida_real; // Reducir la cantidad del servicio afectado por la emergencia

        // Asegurarse de que la cantidad del servicio no sea negativa
        if (actual->cantidad < 0) {
            actual->cantidad = 0;
        }
    }
}

struct Recursos *buscarRecurso(struct Recursos *lista, char *nombre) {
    /*
    Funcionamiento: Busca un recurso en la lista de recursos de una comuna por su nombre
    Entradas: lista (puntero al inicio de la lista de recursos),
              nombre (nombre del recurso a buscar)
    Salidas: puntero al recurso encontrado, o NULL si no se encuentra
    */
    struct Recursos *actual = lista; // Puntero para recorrer la lista de recursos
    int son_iguales = 0; // Variable para almacenar el resultado de la comparación de nombres

    // Recorrer la lista de recursos hasta encontrar el recurso con el nombre especificado
    while (actual != NULL) {
        son_iguales = strcmp(actual->nombre, nombre);
        if (son_iguales == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }

    return NULL;
}

struct Recursos *encontrarRecursoMasBajo(struct Recursos *lista, char *nombre_excluir) {
    /*
    Funcionamiento: Encuentra el recurso con la menor cantidad en la lista de recursos, excluyendo un recurso específico por su nombre
    Entradas: lista (puntero al inicio de la lista de recursos),
              nombre_excluir (nombre del recurso a excluir de la búsqueda)
    Salidas: puntero al recurso con la menor cantidad, o NULL si no se encuentra
    */
    struct Recursos *actual = lista; // Puntero para recorrer la lista de recursos
    struct Recursos *mas_bajo = NULL; // Puntero para almacenar el recurso con la menor cantidad encontrado hasta ahora
    int son_iguales = 0; // Variable para almacenar el resultado de la comparación de nombres

    // Recorrer la lista de recursos para encontrar el recurso con la menor cantidad, excluyendo el recurso especificado
    while (actual != NULL) {
        son_iguales = strcmp(actual->nombre, nombre_excluir);
        if (son_iguales != 0) {
            if (mas_bajo == NULL || actual->cantidad < mas_bajo->cantidad) {
                mas_bajo = actual;
            }
        }
        actual = actual->siguiente;
    }

    return mas_bajo;
}

int puedeDarRecurso(struct Recursos *recurso, int personas, int cantidadARestar) {
    /*
    Funcionamiento: Determina si un recurso puede ser dado sin poner en riesgo a la comuna
    Entradas: recurso (puntero al recurso a evaluar), 
              personas (cantidad de personas en la comuna), 
              cantidadARestar (cantidad del recurso que se desea dar)
    Salidas: 1 si el recurso puede ser dado sin riesgo, 0 si no
    */
    int estimado_1_turno = 0;
    int cantidad_restante = 0;

    if (recurso->emergencias == 1) {
        return 0;
    }

    estimado_1_turno = personas * CONSUMO_PROMEDIO_POR_PERSONA;
    cantidad_restante = recurso->cantidad - cantidadARestar;

    if (cantidad_restante < estimado_1_turno) {
        return 0;
    }

    return 1;
}

struct Servicios *buscarServicio(struct Servicios *lista, char *nombre) {
    /*
    Funcionamiento: Busca un servicio en la lista de servicios de una comuna por su nombre
    Entradas: lista (puntero al inicio de la lista de servicios),
              nombre (nombre del servicio a buscar)
    Salidas: puntero al servicio encontrado, o NULL si no se encuentra
    */
    struct Servicios *actual = lista; // Puntero para recorrer la lista de servicios
    int son_iguales = 0; // Variable para almacenar el resultado de la comparación de nombres

    // Recorrer la lista de servicios hasta encontrar el servicio con el nombre especificado
    while (actual != NULL) {
        son_iguales = strcmp(actual->nombre, nombre);
        if (son_iguales == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }

    return NULL;
}

struct Servicios *encontrarServicioMasBajo(struct Servicios *lista, char *nombre_excluir) {
    /*
    Funcionamiento: Encuentra el servicio con la menor cantidad en la lista de servicios, excluyendo un servicio específico por su nombre
    Entradas: lista (puntero al inicio de la lista de servicios),
              nombre_excluir (nombre del servicio a excluir de la búsqueda)
    Salidas: puntero al servicio con la menor cantidad, o NULL si no se encuentra
    */
    struct Servicios *actual = lista; // Puntero para recorrer la lista de servicios
    struct Servicios *mas_bajo = NULL; // Puntero para almacenar el servicio con la menor cantidad encontrado hasta ahora
    int son_iguales = 0; // Variable para almacenar el resultado de la comparación de nombres

    // Recorrer la lista de servicios para encontrar el de menor cantidad, excluyendo el servicio especificado
    while (actual != NULL) {
        son_iguales = strcmp(actual->nombre, nombre_excluir);
        if (son_iguales != 0) {
            if (mas_bajo == NULL || actual->cantidad < mas_bajo->cantidad) {
                mas_bajo = actual;
            }
        }
        actual = actual->siguiente;
    }

    return mas_bajo;
}

int puedeDarServicio(struct Servicios *lista, struct Servicios *servicio, int personas, int cantidadARestar) {
    /*
    Funcionamiento: Determina si un servicio puede ser dado sin dejar a la comuna en emergencia (misma regla de calcularPuntosServicios)
    Entradas: lista (puntero al inicio de la lista de servicios de la comuna, para contar los tipos),
              servicio (puntero al servicio a evaluar),
              personas (cantidad de personas en la comuna),
              cantidadARestar (cantidad del servicio que se desea dar)
    Salidas: 1 si el servicio puede ser dado sin riesgo, 0 si no
    */
    struct Servicios *actual = lista; // Puntero para recorrer la lista y contar los tipos de servicios
    int num_tipos = 0; // Cantidad de tipos de servicios de la comuna
    int cantidad_restante = 0; // Cantidad que quedaría después de dar el servicio
    float reparto_esperado = 0; // Cantidad esperada de cada servicio por tipo
    float ratio = 0; // Proporción de lo que quedaría respecto al reparto esperado

    // Si el servicio ya está en emergencia, no puede dar
    if (servicio->emergencias == 1) {
        return 0;
    }

    // Una comuna sin personas no puede dar servicios (y asi se evita dividir entre 0 mas abajo)
    if (personas == 0) {
        return 0;
    }

    // Contar el número de tipos de servicios de la comuna
    while (actual != NULL) {
        num_tipos = num_tipos + 1;
        actual = actual->siguiente;
    }

    cantidad_restante = servicio->cantidad - cantidadARestar;
    reparto_esperado = (float)personas / num_tipos;
    ratio = cantidad_restante / reparto_esperado;

    // Si después de dar quedaría en emergencia (menos del 20% del reparto esperado), no puede dar
    if (ratio < 0.2) {
        return 0;
    }

    return 1;
}

int contarPersonasPorOficio(struct Persona *lista, int oficio) {
    /*
    Funcionamiento: Cuenta cuantas personas de una lista tienen un oficio determinado
    Entradas: lista (puntero al inicio de la lista de personas),
              oficio (numero del oficio que se quiere contar)
    Salidas: cantidad de personas con ese oficio
    */
    struct Persona *actual = lista; // Puntero para recorrer la lista de personas
    int cantidad = 0; // Cantidad de personas con ese oficio

    // Recorrer la lista contando las personas que tienen el oficio buscado
    while (actual != NULL) {
        if (actual->oficio == oficio) {
            cantidad = cantidad + 1;
        }
        actual = actual->siguiente;
    }

    return cantidad;
}

void sacarPersonaPorOficio(struct Persona **lista, int oficio, struct Persona **sacada) {
    /*
    Funcionamiento: Saca de la lista a la primera persona que tenga el oficio indicado, sin liberar su memoria
    Entradas: lista (puntero al puntero del inicio de la lista de personas),
              oficio (numero del oficio de la persona que se quiere sacar),
              sacada (puntero donde se guardara la persona sacada)
    Salidas: sacada apunta a la persona sacada (con siguiente en NULL), o a NULL si no habia ninguna con ese oficio
    */
    struct Persona *actual = *lista; // Puntero para recorrer la lista de personas
    struct Persona *anterior = NULL; // Puntero a la persona anterior a la que se esta revisando

    *sacada = NULL;

    // Recorrer la lista hasta encontrar la primera persona con el oficio buscado
    while (actual != NULL) {
        if (actual->oficio == oficio) {
            // Si es la primera de la lista, el inicio pasa a ser la siguiente
            if (anterior == NULL) {
                *lista = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            actual->siguiente = NULL;
            *sacada = actual;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

void trasladarPersonas(struct Comuna *comunaOrigen, struct Comuna *comunaDestino, int oficio, int cantidad) {
    /*
    Funcionamiento: Traslada personas con un oficio determinado de una comuna a otra, por lo que dejan de aparecer en la de origen y pasan a aparecer en la de destino
    Entradas: comunaOrigen (puntero a la comuna de la que salen las personas),
              comunaDestino (puntero a la comuna a la que llegan las personas),
              oficio (numero del oficio de las personas que se trasladan),
              cantidad (cantidad de personas que se trasladan)
    Salidas: ninguna (actualiza directamente las listas de personas de ambas comunas)
    */
    struct Persona *persona_sacada = NULL; // Persona que se saca de la comuna de origen para pasarla a la de destino

    for (int i = 0; i < cantidad; i++) {
        sacarPersonaPorOficio(&comunaOrigen->personas, oficio, &persona_sacada);
        if (persona_sacada != NULL) {
            comunaDestino->personas = insertarPersona(comunaDestino->personas, persona_sacada);
        }
    }
}

void pedirConfirmacion(int *respuesta) {
    /*
    Funcionamiento: Le pregunta al usuario si quiere continuar con el intercambio y lee su respuesta (1 = si, 2 = no)
    Entradas: respuesta (puntero donde se guardara la opcion elegida)
    Salidas: respuesta queda en 1 si acepta, en 2 si no acepta, o en 0 si escribio otra cosa
    */
    char error[100]; // Para descartar lo que escriba el usuario si no es un numero
    int opcion = 0; // Numero que escribio el usuario

    *respuesta = 0;

    printf("Desea continuar con el intercambio? (1 = si, 2 = no): ");
    if (scanf("%d", &opcion) != 1) {
        scanf("%99s", error);
        printf("-----------------------------------------------------------------------------------------------------\n");
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    if (opcion == 1 || opcion == 2) {
        *respuesta = opcion;
    }
}

int validarTrueque(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, int cantidad) {
    /*
    Funcionamiento: Revisa que un trueque tenga sentido antes de hacerlo: que las dos comunas existan, que sean distintas y que la cantidad sea mayor que 0
    Entradas: comunaSolicitante (puntero a la comuna que solicita),
              comunaProveedora (puntero a la comuna que provee),
              cantidad (cantidad que se quiere intercambiar)
    Salidas: 1 si el trueque es valido, 0 si no (e imprime el motivo)
    */
    if (comunaSolicitante == NULL || comunaProveedora == NULL) {
        printf("No se pudo hacer el trueque: falta una de las comunas.\n");
        return 0;
    }

    if (comunaSolicitante == comunaProveedora) {
        printf("No se pudo hacer el trueque: una comuna no puede hacer trueque consigo misma.\n");
        return 0;
    }

    if (cantidad <= 0) {
        printf("No se pudo hacer el trueque: la cantidad debe ser mayor que 0.\n");
        return 0;
    }

    return 1;
}

int calcularBono(int cantidad) {
    /*
    Funcionamiento: Calcula el bono de reciprocidad de un trueque, que es un porcentaje de la cantidad intercambiada redondeado hacia arriba. Representa el valor comunitario extra que se genera cuando dos comunas cooperan
    Entradas: cantidad (cantidad que recibe la comuna en el trueque)
    Salidas: bono (unidades extra que recibe la comuna)
    */
    int bono = 0;

    bono = (cantidad * PORCENTAJE_BONO_TRUEQUE + 99) / 100;

    return bono;
}

int hayRegalo(void) {
    /*
    Funcionamiento: Decide al azar si una emergencia va a recibir un regalo de otra comuna, con la probabilidad PROBABILIDAD_REGALO (en porcentaje)
    Entradas: ninguna
    Salidas: 1 si hay regalo, 0 si no
    */
    int tirada = 0; // Numero al azar entre 0 y 99

    tirada = rand() % 100;
    if (tirada < PROBABILIDAD_REGALO) {
        return 1;
    }

    return 0;
}

int regalarRecursos(struct Comuna *comunaAfectada, int regalo) {
    /*
    Funcionamiento: Recorre las demas comunas una por una, empezando por la siguiente a la afectada, y la primera que pueda regalar sin quedar en emergencia le pasa la cantidad indicada a la comuna afectada, sin pedir nada a cambio. El recurso regalado es el que tiene menos cantidad en la comuna afectada. Solo se regalan recursos, nunca servicios
    Entradas: comunaAfectada (puntero a la comuna que recibe el regalo),
              regalo (cantidad que se regala)
    Salidas: 1 si se hizo el regalo (e imprime un mensaje), 0 si no se pudo (no imprime nada)
    */
    struct Recursos *receptor_recurso = NULL; // Recurso de la comuna afectada que recibe el regalo (el que tiene menos cantidad)
    struct Recursos *donante_recurso = NULL; // Mismo recurso, pero en la comuna que regala
    struct Comuna *donante = NULL; // Comuna que se esta revisando para ver si puede regalar
    int personas_donante = 0; // Cantidad de personas de la comuna que se esta revisando
    int puede_dar = 0; // Indica si la comuna que se esta revisando puede regalar sin quedar en riesgo
    int cantidad_final = 0; // Cantidad que tendria la comuna afectada despues de recibir, para compararla con su maximo

    if (comunaAfectada == NULL || regalo <= 0) {
        return 0;
    }

    // Buscar el recurso con menos cantidad en la comuna afectada (el nombre vacio no excluye ninguno)
    receptor_recurso = encontrarRecursoMasBajo(comunaAfectada->bienes, "");
    if (receptor_recurso == NULL) {
        return 0;
    }

    // Verificar que la comuna afectada no pase de su cantidad maxima al recibir el regalo
    cantidad_final = receptor_recurso->cantidad + regalo;
    if (cantidad_final > receptor_recurso->cantidadMaxima) {
        return 0;
    }

    // Recorrer las demas comunas hasta encontrar una que pueda regalar
    donante = comunaAfectada->siguiente;
    while (donante != comunaAfectada) {
        donante_recurso = buscarRecurso(donante->bienes, receptor_recurso->nombre);
        if (donante_recurso != NULL) {
            personas_donante = recorrerPersonas(donante->personas);
            puede_dar = puedeDarRecurso(donante_recurso, personas_donante, regalo);
            if (puede_dar == 1) {
                donante_recurso->cantidad = donante_recurso->cantidad - regalo;
                receptor_recurso->cantidad = receptor_recurso->cantidad + regalo;
                printf("%s le regalo %d de %s a %s, sin pedir nada a cambio.\n", donante->nombre, regalo, receptor_recurso->nombre, comunaAfectada->nombre);
                return 1;
            }
        }
        donante = donante->siguiente;
    }

    return 0;
}
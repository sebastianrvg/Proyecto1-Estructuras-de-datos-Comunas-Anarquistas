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
        estimado_1_turno = 0;
        // Estimar la cantidad de recursos necesarios para 1 turno y luego multiplicar por 2
        for (int i = 0; i < personas; i++) {
            estimado_1_turno += (rand() % 4) + 1; // Simulación de consumo de recursos por persona (1 a 4 unidades)
        }
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
    perdida (cantidad de unidades perdidas), 
            nombre_emergencia (nombre de la emergencia)
    Salidas: ninguna (actualiza directamente los bienes o servicios de la comuna)
    */
    int bien_seleccionado = 0;
    int cantidadNodos = 0;
    int indice = 0;

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

        if (actual->emergencias == 1) {
            printf("Ha ocurrido %s en %s. El bien %s ya estaba en emergencia y perdio %d unidades mas.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida);
        } else {
            printf("Ha ocurrido %s en %s. Se vio afectado el bien %s, se perdieron %d unidades.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida);
        }

        actual->cantidad = actual->cantidad - perdida;
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

        // Si el servicio ya estaba en emergencia, informar que perdió más unidades
        if (actual->emergencias == 1) {
            printf("Ha ocurrido %s en %s. El servicio %s ya estaba en emergencia y perdio %d unidades mas.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida);
        
        // Si el servicio no estaba en emergencia, informar que se vio afectado y perdió unidades
        } else {
            printf("Ha ocurrido %s en %s. Se vio afectado el servicio %s, se perdieron %d unidades.\n", nombre_emergencia, comuna->nombre, actual->nombre, perdida);
        }

        // Actualizar la cantidad del servicio afectado por la emergencia
        actual->cantidad = actual->cantidad - perdida; // Reducir la cantidad del servicio afectado por la emergencia

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

    estimado_1_turno = personas * ((rand() % 3) + 1);
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
    Salidas: respuesta queda en 1 si acepta, en 2 si no acepta, o en 0 si escribio otra cosa o ya no hay mas datos que leer
    */
    char texto[20]; // Texto que escribe el usuario
    char *leido = NULL; // Resultado de leer el texto (NULL si ya no hay mas datos)
    int opcion = 0; // Numero que escribio el usuario
    int convertidos = 0; // Cantidad de numeros que se pudieron leer del texto

    *respuesta = 0;

    printf("Desea continuar con el intercambio? (1 = si, 2 = no): ");
    leido = fgets(texto, sizeof(texto), stdin);
    if (leido == NULL) {
        printf("\n");
        return;
    }
    convertidos = sscanf(texto, "%d", &opcion);
    if (convertidos == 1) {
        if (opcion == 1 || opcion == 2) {
            *respuesta = opcion;
        }
    }
}
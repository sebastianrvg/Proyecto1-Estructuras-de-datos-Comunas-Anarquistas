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
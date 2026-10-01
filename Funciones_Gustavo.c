#include <stdio.h>
#include <stdlib.h>
#include "Funciones_Gustavo.h"
#include "FuncionesAux_Gustavo.h"

float formulaSatisfaccion(struct Comuna *comuna) {
    /*
    funcionamiento: Calcula la satisfacción de una comuna en base a sus recursos y servicios disponibles, así como la cantidad de emergencias que se han producido
    entradas: comuna (puntero a la comuna de la cual se quiere calcular la satisfacción)
    salidas: satisfacción
    */
    struct Recursos *r = comuna->bienes;
    struct Servicios *s = comuna->servicios;
    int personas = 0;
    int total_recursos = 0;
    int total_servicios = 0;
    int suma_puntos_recursos = 0;
    int emergencias_recursos = 0;
    int suma_puntos_servicios = 0;
    int emergencias_servicios = 0;
    int total_emergencias = 0;
    int resultados_validos = 0;
    float tope = 0;
    float valores_funcionales = 0;
    float satisfaccion = 0;

    personas = recorrerPersonas(comuna->personas);

    // Contar el total de recursos
    while (r != NULL) {
        total_recursos = total_recursos + 1;
        r = r->siguiente;
    }

    // Contar el total de servicios
    while (s != NULL) {
        total_servicios = total_servicios + 1;
        s = s->siguiente;
    }

    // Calcular los puntos y emergencias para recursos y servicios
    calcularPuntosRecursos(comuna->bienes, personas, &suma_puntos_recursos, &emergencias_recursos);
    calcularPuntosServicios(comuna->servicios, personas, &suma_puntos_servicios, &emergencias_servicios);

    // Calcular el total de emergencias
    total_emergencias = emergencias_recursos + emergencias_servicios;

    // Calcular el tope de satisfacción basado en el número de emergencias
    if (total_emergencias == 0) {
        tope = 99;
    } else {
        tope = 50 - 5 * (total_emergencias - 1);
        if (tope < 0) {
            tope = 0;
        }
    }

    // Calcular los resultados válidos (total de recursos y servicios menos las emergencias)
    resultados_validos = (total_recursos - emergencias_recursos) + (total_servicios - emergencias_servicios);

    if (resultados_validos == 0) {
        satisfaccion = tope;
    } else {
        valores_funcionales = (float)(suma_puntos_recursos + suma_puntos_servicios) / (2 * resultados_validos);
        satisfaccion = valores_funcionales * tope;
    }

    return satisfaccion;
}

float calcularSatisfaccion(struct Comuna *actual, int cantidadComunas) {
    /*
    Funcionamiento: Calcula la satisfacción de una comuna en base a su propia satisfacción y la de sus comunas vecinas
    Entradas: actual (puntero a la comuna de la cual se quiere calcular la satisfacción y cantidadComunas (número total de comunas en la lista circular)
    Salidas: satisfacción final
    */
    float propia = 0;
    float suma_vecinas = 0;
    int cantidad_vecinas = 0;
    float promedio_vecinas = 0;
    float satisfaccion_final = 0;

    // Calcular la satisfacción propia de la comuna actual
    propia = formulaSatisfaccion(actual);

    // Calcular la satisfacción de las comunas vecinas
    if (cantidadComunas >= 5) { // Si hay 5 o más comunas, se consideran las 4 vecinas más cercanas
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 2, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 2, cantidadComunas));
        cantidad_vecinas = 4;
    } else if (cantidadComunas == 4) { // Si hay 4 comunas, se consideran las 3 vecinas más cercanas
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 2, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        cantidad_vecinas = 3;
    } else if (cantidadComunas == 3) { // Si hay 3 comunas, se consideran las 2 vecinas más cercanas
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        cantidad_vecinas = 2;
    } else if (cantidadComunas == 2) { // Si hay 2 comunas, se considera la única vecina
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        cantidad_vecinas = 1;
    }

    if (cantidad_vecinas == 0) { // Si no hay comunas vecinas, la satisfacción final es la propia
        satisfaccion_final = propia;

    // Si hay comunas vecinas, se calcula el promedio de sus satisfacciones y se combina con la propia
    } else {
        promedio_vecinas = suma_vecinas / cantidad_vecinas;
        satisfaccion_final = propia * 0.7 + promedio_vecinas * 0.3;
    }

    // Actualizar la satisfacción de la comuna actual
    actual->satisfaccion = satisfaccion_final;

    return satisfaccion_final;
}

float calcularNecesidad(struct Comuna *comuna) {
    /*
    Funcionamiento: Calcula la necesidad de una comuna en base a sus recursos y servicios disponibles
    Entradas: comuna (puntero a la comuna de la cual se quiere calcular la necesidad)
    Salidas: necesidad
    */
    struct Recursos *r = comuna->bienes;
    struct Servicios *s = comuna->servicios;
    int suma_maximo_recursos = 0;
    int suma_cantidad_recursos = 0;
    int suma_maximo_servicios = 0;
    int suma_cantidad_servicios = 0;
    float faltante_recursos = 0;
    float faltante_servicios = 0;
    float promedio_faltantes = 0;
    float necesidad = 0;

    // Calcular la suma de las cantidades máximas y actuales de recursos
    while (r != NULL) {
        suma_maximo_recursos = suma_maximo_recursos + r->cantidadMaxima;
        suma_cantidad_recursos = suma_cantidad_recursos + r->cantidad;
        r = r->siguiente;
    }

    // Calcular la suma de las cantidades máximas y actuales de servicios
    while (s != NULL) {
        suma_maximo_servicios = suma_maximo_servicios + s->cantidadMaxima;
        suma_cantidad_servicios = suma_cantidad_servicios + s->cantidad;
        s = s->siguiente;
    }

    // Calcular el faltante de recursos y servicios como proporción de la cantidad máxima
    faltante_recursos = (float)(suma_maximo_recursos - suma_cantidad_recursos) / suma_maximo_recursos;
    faltante_servicios = (float)(suma_maximo_servicios - suma_cantidad_servicios) / suma_maximo_servicios;

    // Calcular el promedio de los faltantes de recursos y servicios
    promedio_faltantes = (faltante_recursos + faltante_servicios) / 2;

    necesidad = 99 * promedio_faltantes;

    // Actualizar la necesidad de la comuna
    comuna->necesidad = necesidad;

    return necesidad;
}

void actualizarIndices(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: recorre todas las comunas de la lista circular y les
    actualiza su necesidad y satisfaccion usando las formulas.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
    cantidadComunas (numero total de comunas en la lista)
    Salidas: ninguna (actualiza directamente cada comuna->necesidad y comuna->satisfaccion)
    */
    struct Comuna *actual = inicio;
    struct Comuna *comuna_inicial = inicio;

    while (1) {
        calcularNecesidad(actual);
        calcularSatisfaccion(actual, cantidadComunas);

        actual = actual->siguiente;

        if (actual == comuna_inicial) {
            break;
        }
    }
}

void aplicarConsumo(struct Comuna *comuna) {
    /*
    Funcionamiento: reduce la cantidad de cada bien de la comuna segun lo que
    consumieron sus personas en el turno.
    Entradas: comuna (puntero a la comuna que va a consumir sus bienes)
    Salidas: ninguna (actualiza directamente cada bien->cantidad)
    */
    struct Recursos *actual = comuna->bienes;
    int personas = 0;
    int consumo = 0;

    personas = recorrerPersonas(comuna->personas);

    while (actual != NULL) {
        consumo = 0;
        for (int i = 0; i < personas; i++) {
            consumo = consumo + (rand() % 5) + 1;
        }

        actual->cantidad = actual->cantidad - consumo;
        if (actual->cantidad < 0) {
            actual->cantidad = 0;
        }

        actual = actual->siguiente;
    }
}

void aplicarConsumoTodas(struct Comuna *inicio) {
    /*
    Funcionamiento: recorre todas las comunas de la lista circular y les
    aplica el consumo de bienes a cada una.
    Entradas: inicio (puntero a cualquier comuna de la lista circular)
    Salidas: ninguna (actualiza directamente los bienes de cada comuna)
    */
    struct Comuna *actual = inicio;
    struct Comuna *comuna_inicial = inicio;

    while (1) {
        aplicarConsumo(actual);

        actual = actual->siguiente;

        if (actual == comuna_inicial) {
            break;
        }
    }
}

void pasarTurno(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: aplica el consumo de bienes a todas las comunas y luego
    actualiza sus indices de necesidad y satisfaccion, en ese orden. Se usa
    cuando el jugador pasa al siguiente dia.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
    cantidadComunas (numero total de comunas en la lista)
    Salidas: ninguna
    */
    aplicarConsumoTodas(inicio);
    actualizarIndices(inicio, cantidadComunas);
}
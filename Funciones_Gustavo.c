#include <stdio.h>
#include <stdlib.h>
#include "Funciones_Gustavo.h"
#include "FuncionesAux_Gustavo.h"

float formulaSatisfaccion(struct Comuna *comuna) {
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

    while (r != NULL) {
        total_recursos = total_recursos + 1;
        r = r->siguiente;
    }

    while (s != NULL) {
        total_servicios = total_servicios + 1;
        s = s->siguiente;
    }

    calcularPuntosRecursos(comuna->bienes, personas, &suma_puntos_recursos, &emergencias_recursos);
    calcularPuntosServicios(comuna->servicios, personas, &suma_puntos_servicios, &emergencias_servicios);

    total_emergencias = emergencias_recursos + emergencias_servicios;

    if (total_emergencias == 0) {
        tope = 99;
    } else {
        tope = 50 - 5 * (total_emergencias - 1);
        if (tope < 0) {
            tope = 0;
        }
    }

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
    float propia = 0;
    float suma_vecinas = 0;
    int cantidad_vecinas = 0;
    float promedio_vecinas = 0;
    float satisfaccion_final = 0;

    propia = formulaSatisfaccion(actual);

    if (cantidadComunas >= 5) {
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 2, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 2, cantidadComunas));
        cantidad_vecinas = 4;
    } else if (cantidadComunas == 4) {
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 2, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        cantidad_vecinas = 3;
    } else if (cantidadComunas == 3) {
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas));
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        cantidad_vecinas = 2;
    } else if (cantidadComunas == 2) {
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas));
        cantidad_vecinas = 1;
    }

    if (cantidad_vecinas == 0) {
        satisfaccion_final = propia;
    } else {
        promedio_vecinas = suma_vecinas / cantidad_vecinas;
        satisfaccion_final = propia * 0.7 + promedio_vecinas * 0.3;
    }

    actual->satisfaccion = satisfaccion_final;

    return satisfaccion_final;
}
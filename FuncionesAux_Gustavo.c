#include <stdio.h>
#include <stdlib.h>
#include "FuncionesAux_Gustavo.h"

void calcularPuntosRecursos(struct Recursos *lista, int personas, int *suma_puntos, int *cantidad_emergencias) {
    struct Recursos *actual = lista;
    int estimado_1_turno = 0;
    int estimado_2_turnos = 0;
    int puntos = 0;
    float margen = 0;

    *suma_puntos = 0;
    *cantidad_emergencias = 0;

    while (actual != NULL) {
        estimado_1_turno = 0;
        for (int i = 0; i < personas; i++) {
            estimado_1_turno += (rand() % 5) + 1;
        }
        estimado_2_turnos = estimado_1_turno * 2;

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

        if (puntos != -1) {
            *suma_puntos = *suma_puntos + puntos;
        }

        actual = actual->siguiente;
    }
}

void calcularPuntosServicios(struct Servicios *lista, int personas, int *suma_puntos, int *cantidad_emergencias) {
    struct Servicios *actual = lista;
    int num_tipos = 0;
    int puntos = 0;
    float reparto_esperado = 0;
    float ratio = 0;

    *suma_puntos = 0;
    *cantidad_emergencias = 0;

    while (actual != NULL) {
        num_tipos = num_tipos + 1;
        actual = actual->siguiente;
    }

    if (num_tipos == 0) {
        return;
    }

    reparto_esperado = (float)personas / num_tipos;

    actual = lista;
    while (actual != NULL) {
        ratio = actual->cantidad / reparto_esperado;

        if (ratio < 0.2) {
            puntos = -1;
            (*cantidad_emergencias)++;
        } else if (ratio <= 0.6) {
            puntos = 0;
        } else if (ratio < 1) {
            puntos = 1;
        } else {
            puntos = 2;
        }

        if (puntos != -1) {
            *suma_puntos = *suma_puntos + puntos;
        }

        actual = actual->siguiente;
    }
}
#include <stdlib.h>
#include "Consumo.h"

void aplicarConsumo(struct Comuna *comuna) {
    /*
    Funcionamiento: reduce la cantidad de cada bien de la comuna según lo que consumieron sus personas en el turno.
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
            consumo = consumo + (rand() % (CONSUMO_MAXIMO_POR_PERSONA - CONSUMO_MINIMO_POR_PERSONA + 1)) + CONSUMO_MINIMO_POR_PERSONA;
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
    Funcionamiento: recorre todas las comunas de la lista circular y les aplica el consumo de bienes a cada una.
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

#include <stdio.h>
#include <stdlib.h>
#include "PasarDia.h"
#include "Consumo.h"
#include "Indices.h"
#include "Emergencias.h"
#include "Computadora.h"
#include "Aprendizaje.h"
#include "Sociedad.h"
#include "FuncionAdicional.h"

void pasarTurno(struct Comuna **inicio, int *cantidadComunas, int *turnosHastaEmergencia, struct Comuna **comunaActual) {
    /*
    Funcionamiento: aplica el consumo de bienes a todas las comunas, luego actualiza sus índices de necesidad y satisfacción, controla si ocurre una emergencia en alguna comuna, vuelve a actualizar los índices para que reflejen la emergencia y el posible regalo, y por último revisa si alguna comuna debe disolverse.
    Entradas: inicio (puntero al puntero de cualquier comuna de la lista circular),
              cantidadComunas (puntero a la cantidad total de comunas),
              turnosHastaEmergencia (puntero al contador de turnos para la proxima emergencia),
              comunaActual (puntero a la comuna administrada seleccionada por el usuario)
    Salidas: ninguna
    */
    struct Comuna *actual = NULL;
    struct Comuna *siguiente = NULL;
    int totalOriginal = *cantidadComunas; // Cuantas comunas habia al empezar el turno, para no revisar mas ni menos de las que habia

    aplicarConsumoTodas(*inicio);
    actualizarIndices(*inicio, *cantidadComunas);
    producirBienes(*inicio, *cantidadComunas);
    printf("---------------------------------------- Trueques autonomos -----------------------------------------\n");
    trueque_computadora(*inicio, *cantidadComunas);
    leerEnBiblioteca(*inicio, *cantidadComunas);
    controlarEmergencias(*inicio, *cantidadComunas, turnosHastaEmergencia);
    actualizarIndices(*inicio, *cantidadComunas); // Otra vez, para que los indices muestren lo que dejo la emergencia y el posible regalo

    // Revisar cada comuna que habia al empezar el turno y disolver las que corresponda.
    // Se guarda "siguiente" antes de revisar "actual" porque disolverComuna puede liberar "actual"
    actual = *inicio;
    for (int i = 0; i < totalOriginal; i++) {
        siguiente = actual->siguiente;

        if (comunaDebeDisolverse(actual)) {
            *inicio = disolverComuna(*inicio, cantidadComunas, actual, comunaActual);
        }

        actual = siguiente;
    }
}
#include "PasarDia.h"
#include "Consumo.h"
#include "Indices.h"
#include "Emergencias.h"

void pasarTurno(struct Comuna *inicio, int cantidadComunas, int *turnosHastaEmergencia) {
    /*
    Funcionamiento: aplica el consumo de bienes a todas las comunas, luego actualiza sus índices de necesidad y satisfacción, controla si ocurre una emergencia en alguna comuna y vuelve a actualizar los índices para que reflejen la emergencia y el posible regalo.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (número total de comunas en la lista)
    Salidas: ninguna
    */
    aplicarConsumoTodas(inicio);
    actualizarIndices(inicio, cantidadComunas);
    controlarEmergencias(inicio, cantidadComunas, turnosHastaEmergencia);
    actualizarIndices(inicio, cantidadComunas); // Otra vez, para que los indices muestren lo que dejo la emergencia y el posible regalo
}
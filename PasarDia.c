#include "PasarDia.h"
#include "Consumo.h"
#include "Indices.h"
#include "Emergencias.h"

void pasarTurno(struct Comuna *inicio, int cantidadComunas, int *turnosHastaEmergencia) {
    /*
    Funcionamiento: aplica el consumo de bienes a todas las comunas, luego actualiza sus índices de necesidad y satisfacción y finalmente controla si ocurre una emergencia en alguna comuna.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (número total de comunas en la lista)
    Salidas: ninguna
    */
    aplicarConsumoTodas(inicio);
    actualizarIndices(inicio, cantidadComunas);
    controlarEmergencias(inicio, cantidadComunas, turnosHastaEmergencia);
}

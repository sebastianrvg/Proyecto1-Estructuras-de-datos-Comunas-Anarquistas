#include <stdio.h>
#include <stdlib.h>
#include "Computadora.h"
#include "Trueque.h"
#include "Busqueda.h"
#include "Indices.h"

int trueque_bienes_computadora(struct Comuna *solicitante, struct Comuna *proveedora, char *nombre_buscado, int cantidad_buscada) {
    /*
    Funcionamiento: igual que truequeBienes (mismas reglas, mismo bono y mismo azar) pero para una comuna autonoma: no pide confirmacion ni imprime nada. Las dos comunas deben ser distintas y la cantidad mayor que 0
    Entradas: solicitante (comuna autonoma que pide),
              proveedora (comuna que da el bien),
              nombre_buscado (nombre del bien que se pide),
              cantidad_buscada (cantidad que se pide)
    Salidas: 1 si el trueque se hizo, 0 si no (actualiza directamente los bienes de ambas comunas)
    */
    struct Recursos *B_recurso_proveedora = NULL; // Bien que la solicitante quiere, en la proveedora
    struct Recursos *B_recurso_pedido = NULL; // Bien con menos cantidad de la proveedora, el que pide a cambio
    struct Recursos *A_recurso_solicitante = NULL; // Bien que la solicitante da a cambio
    struct Recursos *A_recurso_solicitante_recibe = NULL; // Bien que la solicitante recibe
    int cantidad_pedida = 0; // Cantidad que la solicitante da a cambio
    int cantidad_final = 0; // Cantidad que tendria una comuna despues de recibir, para compararla con su maximo
    int A_bono_solicitante = 0;
    int B_bono_proveedora = 0;

    B_recurso_proveedora = buscarRecurso(proveedora->bienes, nombre_buscado);
    if (B_recurso_proveedora == NULL) {
        return 0;
    }

    if (puedeDarRecurso(B_recurso_proveedora, recorrerPersonas(proveedora->personas), cantidad_buscada) == 0) {
        return 0;
    }

    B_recurso_pedido = encontrarRecursoMasBajo(proveedora->bienes, nombre_buscado);
    if (B_recurso_pedido == NULL) {
        return 0;
    }

    // Igual que en el trueque normal, 25% de probabilidad de que pida de mas
    cantidad_pedida = cantidad_buscada;
    if (rand() % 4 == 0) {
        cantidad_pedida = cantidad_pedida + (rand() % 4) + 2;
    }

    A_recurso_solicitante = buscarRecurso(solicitante->bienes, B_recurso_pedido->nombre);
    if (A_recurso_solicitante == NULL) {
        return 0;
    }

    if (puedeDarRecurso(A_recurso_solicitante, recorrerPersonas(solicitante->personas), cantidad_pedida) == 0) {
        return 0;
    }

    A_recurso_solicitante_recibe = buscarRecurso(solicitante->bienes, B_recurso_proveedora->nombre);
    if (A_recurso_solicitante_recibe == NULL) {
        return 0;
    }

    // Ninguna de las dos puede pasar de su cantidad maxima
    cantidad_final = A_recurso_solicitante_recibe->cantidad + cantidad_buscada;
    if (cantidad_final > A_recurso_solicitante_recibe->cantidadMaxima) {
        return 0;
    }
    cantidad_final = B_recurso_pedido->cantidad + cantidad_pedida;
    if (cantidad_final > B_recurso_pedido->cantidadMaxima) {
        return 0;
    }

    A_bono_solicitante = calcularBono(cantidad_buscada);
    B_bono_proveedora = calcularBono(cantidad_pedida);

    B_recurso_proveedora->cantidad = B_recurso_proveedora->cantidad - cantidad_buscada;

    A_recurso_solicitante_recibe->cantidad = A_recurso_solicitante_recibe->cantidad + cantidad_buscada + A_bono_solicitante;
    if (A_recurso_solicitante_recibe->cantidad > A_recurso_solicitante_recibe->cantidadMaxima) {
        A_recurso_solicitante_recibe->cantidad = A_recurso_solicitante_recibe->cantidadMaxima;
    }

    A_recurso_solicitante->cantidad = A_recurso_solicitante->cantidad - cantidad_pedida;

    B_recurso_pedido->cantidad = B_recurso_pedido->cantidad + cantidad_pedida + B_bono_proveedora;
    if (B_recurso_pedido->cantidad > B_recurso_pedido->cantidadMaxima) {
        B_recurso_pedido->cantidad = B_recurso_pedido->cantidadMaxima;
    }

    return 1;
}

int pedir_a_todas(struct Comuna *solicitante, struct Recursos *recurso, int cantidad) {
    /*
    Funcionamiento: le pide esa cantidad a cada otra comuna autonoma, en orden, hasta que una acepte
    Entradas: solicitante (comuna autonoma que pide),
              recurso (bien que pide),
              cantidad (cuanto pide)
    Salidas: 1 si alguna acepto, 0 si ninguna
    */
    struct Comuna *proveedora = solicitante->siguiente;

    while (proveedora != solicitante) {
        if (proveedora->administrado == 0 && trueque_bienes_computadora(solicitante, proveedora, recurso->nombre, cantidad) == 1) {
            return 1;
        }
        proveedora = proveedora->siguiente;
    }

    return 0;
}

int bajar_cantidad(int cantidad) {
    /*
    Funcionamiento: calcula la cantidad que se pide en la siguiente ronda: baja 2, y si queda en 0 o menos pide 1
    Entradas: cantidad (lo que se pidio en esta ronda)
    Salidas: la nueva cantidad, o 0 si ya se intento con 1 (se acabaron los intentos)
    */
    if (cantidad == 1) {
        return 0;
    }
    if (cantidad <= 2) {
        return 1;
    }

    return cantidad - 2;
}

int pedir_recurso_computadora(struct Comuna *solicitante, struct Recursos *recurso, int objetivo) {
    /*
    Funcionamiento: una comuna autonoma pide un bien a las demas autonomas hasta llegar a la cantidad objetivo. Primero pide todo lo que le falta; si ninguna acepta, baja de 2 en 2 lo que pide (si queda en 0 o menos pide 1). Cuando un trueque se hace, calcula lo que aun le falta y repite. Si nadie acepta ni 1 unidad, termina
    Entradas: solicitante (comuna autonoma que pide),
              recurso (bien de la solicitante que quiere aumentar),
              objetivo (cantidad con la que quiere quedar)
    Salidas: cantidad de trueques que se hicieron
    */
    int falta = objetivo - recurso->cantidad; // Unidades que le faltan para llegar al objetivo
    int pedir = 0; // Cantidad que se intenta pedir
    int trueques = 0;

    while (falta > 0) {
        pedir = falta;
        while (pedir > 0 && pedir_a_todas(solicitante, recurso, pedir) == 0) {
            pedir = bajar_cantidad(pedir);
        }
        if (pedir == 0) {
            break; // Nadie acepto ni 1 unidad
        }
        trueques = trueques + 1;
        falta = objetivo - recurso->cantidad;
    }

    return trueques;
}

void trueque_computadora(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: cada comuna autonoma (administrado = 0) pide primero su bien con menos cantidad y despues cada bien que siga en emergencia, hasta quedar con 2 turnos de consumo mas 1. Solo hace trueques con otras autonomas. Imprime una linea por cada bien que logro pedir
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (actualiza los bienes y los indices)
    */
    struct Comuna *actual = inicio;
    struct Recursos *mas_bajo = NULL; // Bien con menos cantidad de la comuna
    struct Recursos *recurso = NULL;
    int objetivo = 0; // Cantidad con la que sale de emergencia y uno mas (2 turnos de consumo + 1)
    int meta = 0; // Objetivo del bien mas bajo (si ya lo supera, pide solo uno mas)
    int trueques = 0; // Trueques hechos para un bien
    int total = 0; // Trueques hechos por la comuna

    for (int i = 0; i < cantidadComunas; i++) {
        if (actual->administrado == 0) {
            total = 0;
            objetivo = recorrerPersonas(actual->personas) * CONSUMO_PROMEDIO_POR_PERSONA * 2 + 1;

            // El bien con menos cantidad siempre se pide, aunque no este en emergencia
            mas_bajo = encontrarRecursoMasBajo(actual->bienes, "");
            if (mas_bajo != NULL) {
                meta = objetivo;
                if (meta <= mas_bajo->cantidad) {
                    meta = mas_bajo->cantidad + 1;
                }
                trueques = pedir_recurso_computadora(actual, mas_bajo, meta);
                if (trueques > 0) {
                    printf("La comuna autonoma %s hizo %d trueque(s) para conseguir %s.\n", actual->nombre, trueques, mas_bajo->nombre);
                }
                total = total + trueques;
            }

            // Despues, cada bien que siga en emergencia
            recurso = actual->bienes;
            while (recurso != NULL) {
                if (recurso->emergencias == 1 && recurso != mas_bajo && recurso->cantidad < objetivo) {
                    trueques = pedir_recurso_computadora(actual, recurso, objetivo);
                    if (trueques > 0) {
                        printf("La comuna autonoma %s hizo %d trueque(s) para conseguir %s.\n", actual->nombre, trueques, recurso->nombre);
                    }
                    total = total + trueques;
                }
                recurso = recurso->siguiente;
            }

            // Actualizar los indices para que la siguiente comuna vea las emergencias reales
            if (total > 0) {
                actualizarIndices(inicio, cantidadComunas);
            }
        }
        actual = actual->siguiente;
    }
}
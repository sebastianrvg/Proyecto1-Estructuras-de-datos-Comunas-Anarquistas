#include <stdio.h>
#include <string.h>
#include "FuncionAdicional.h"
#include "Busqueda.h"
#include "Colores.h"

int ajustePorNivel(float valor) {
    /*
    Funcionamiento: convierte un indice de 0 a 99 en un ajuste de produccion: si esta en el nivel bajo resta AJUSTE_PRODUCCION, en el medio no cambia nada y en el alto suma AJUSTE_PRODUCCION
    Entradas: valor (necesidad o satisfaccion de la comuna)
    Salidas: el ajuste en porcentaje (-AJUSTE_PRODUCCION, 0 o AJUSTE_PRODUCCION)
    */
    if (valor < NIVEL_BAJO) {
        return -AJUSTE_PRODUCCION;
    }
    if (valor > NIVEL_ALTO) {
        return AJUSTE_PRODUCCION;
    }

    return 0;
}

struct Recursos *bienEnPosicion(struct Recursos *bienes, int posicion) {
    /*
    Funcionamiento: busca el bien que esta en una posicion de la lista (la primera posicion es 1)
    Entradas: bienes (lista de bienes de la comuna), posicion (posicion que se busca)
    Salidas: el bien en esa posicion, o NULL si la lista es mas corta
    */
    struct Recursos *actual = bienes;

    for (int i = 1; i < posicion && actual != NULL; i++) {
        actual = actual->siguiente;
    }

    return actual;
}

int calcularProduccion(struct Comuna *comuna, int trabajadores, int cantidadServicios) {
    /*
    Funcionamiento: calcula cuanto producen las personas de un oficio en un dia. La produccion base sube con la necesidad alta (trabajan mas porque les falta) y con la satisfaccion alta, y baja con la necesidad baja y con la satisfaccion baja
    Entradas: comuna (comuna que produce), trabajadores (personas con ese oficio),
              cantidadServicios (servicios de la simulacion)
    Salidas: unidades producidas en el dia
    */
    int base = trabajadores * PRODUCCION_BASE * cantidadServicios;
    int porcentaje = 100 + ajustePorNivel(comuna->necesidad) + ajustePorNivel(comuna->satisfaccion);

    return base * porcentaje / 100;
}

int regalarExcedente(struct Comuna *donante, struct Recursos *bien, int excedente) {
    /*
    Funcionamiento: reparte lo que se produjo de mas entre las demas comunas, en orden desde la siguiente. Cada comuna recibe solo lo que le cabe sin pasar de su maximo. Lo que nadie pueda recibir se pierde
    Entradas: donante (comuna que produjo de mas), bien (el bien que sobro, de la donante),
              excedente (unidades que sobraron)
    Salidas: unidades que se regalaron
    */
    struct Comuna *receptora = donante->siguiente;
    struct Recursos *bien_receptora = NULL;
    int espacio = 0; // Lo que le cabe a la comuna receptora
    int dar = 0; // Lo que se le regala a la comuna receptora
    int regalado = 0;

    while (receptora != donante && excedente > 0) {
        bien_receptora = buscarRecurso(receptora->bienes, bien->nombre);
        if (bien_receptora != NULL) {
            espacio = bien_receptora->cantidadMaxima - bien_receptora->cantidad;
            dar = excedente;
            if (dar > espacio) {
                dar = espacio;
            }
            if (dar > 0) {
                bien_receptora->cantidad = bien_receptora->cantidad + dar;
                donante->regalosDados = donante->regalosDados + 1;
                receptora->regalosRecibidos = receptora->regalosRecibidos + 1;
                excedente = excedente - dar;
                regalado = regalado + dar;
            }
        }
        receptora = receptora->siguiente;
    }

    return regalado;
}

void mostrarProduccion(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: muestra, para cada comuna, cuanto produjo de cada bien en el ultimo dia y con cuanto quedo. Solo muestra los bienes que se produjeron
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (imprime en pantalla)
    */
    struct Comuna *comuna = inicio;
    struct Recursos *bien = NULL;

    printf("--------------------------------------------- Produccion --------------------------------------------\n");
    for (int i = 0; i < cantidadComunas; i++) {
        printf("%s:\n", comuna->nombre);
        bien = comuna->bienes;
        while (bien != NULL) {
            if (bien->producidoHoy > 0) {
                printf("  %s: +%d, quedo en %d de %d\n", bien->nombre, bien->producidoHoy, bien->cantidad, bien->cantidadMaxima);
            }
            bien = bien->siguiente;
        }
        comuna = comuna->siguiente;
    }
}

void producirBienes(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: recorre los servicios en orden (servicio 1, 2, ...) y en cada comuna cuenta las personas con ese oficio, que producen el bien de la misma posicion. Lo que no cabe en la comuna se le regala a otras. Imprime un resumen del dia
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (actualiza los bienes de las comunas)
    */
    struct Comuna *comuna = NULL;
    struct Servicios *servicio = NULL;
    struct Recursos *bien = NULL;
    int cantidadServicios = recorrerServicios(inicio->servicios);
    int trabajadores = 0;
    int producido = 0; // Lo que produjo una comuna de un bien
    int excedente = 0; // Lo que no le cupo a la comuna
    int total_producido = 0;
    int total_regalado = 0;

    // Borrar lo que se produjo el dia anterior
    comuna = inicio;
    for (int i = 0; i < cantidadComunas; i++) {
        for (bien = comuna->bienes; bien != NULL; bien = bien->siguiente) {
            bien->producidoHoy = 0;
        }
        comuna = comuna->siguiente;
    }

    for (int posicion = 1; posicion <= cantidadServicios; posicion++) {
        comuna = inicio;
        for (int i = 0; i < cantidadComunas; i++) {
            servicio = comuna->servicios;
            for (int j = 1; j < posicion && servicio != NULL; j++) {
                servicio = servicio->siguiente;
            }
            bien = bienEnPosicion(comuna->bienes, posicion);

            if (servicio != NULL && bien != NULL) {
                trabajadores = contarPersonasPorOficio(comuna->personas, servicio->oficio);
                producido = calcularProduccion(comuna, trabajadores, cantidadServicios);
                total_producido = total_producido + producido;
                bien->producidoHoy = producido;

                bien->cantidad = bien->cantidad + producido;
                if (bien->cantidad > bien->cantidadMaxima) {
                    excedente = bien->cantidad - bien->cantidadMaxima;
                    bien->cantidad = bien->cantidadMaxima;
                    total_regalado = total_regalado + regalarExcedente(comuna, bien, excedente);
                }
            }
            comuna = comuna->siguiente;
        }
    }

    mostrarProduccion(inicio, cantidadComunas);
    printf(COLOR_VERDE "Las comunas produjeron %d unidades de bienes y regalaron %d de excedente.\n" COLOR_RESET, total_producido, total_regalado);
}
#include <stdio.h>
#include <stdlib.h>
#include "Trueque.h"
#include "Busqueda.h"
#include "Colores.h"

int validarTrueque(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, int cantidad) {
    /*
    Funcionamiento: Revisa que un trueque tenga sentido antes de hacerlo: que las dos comunas existan, que sean distintas y que la cantidad sea mayor que 0
    Entradas: comunaSolicitante (puntero a la comuna que solicita),
              comunaProveedora (puntero a la comuna que provee),
              cantidad (cantidad que se quiere intercambiar)
    Salidas: 1 si el trueque es valido, 0 si no (e imprime el motivo)
    */
    if (comunaSolicitante == NULL || comunaProveedora == NULL) {
        printf("No se pudo hacer el trueque: falta una de las comunas.\n");
        return 0;
    }

    if (comunaSolicitante == comunaProveedora) {
        printf("No se pudo hacer el trueque: una comuna no puede hacer trueque consigo misma.\n");
        return 0;
    }

    if (cantidad <= 0) {
        printf("No se pudo hacer el trueque: la cantidad debe ser mayor que 0.\n");
        return 0;
    }

    return 1;
}

int calcularBono(int cantidad) {
    /*
    Funcionamiento: Calcula el bono de reciprocidad de un trueque, que es un porcentaje de la cantidad intercambiada redondeado hacia arriba. Representa el valor comunitario extra que se genera cuando dos comunas cooperan
    Entradas: cantidad (cantidad que recibe la comuna en el trueque)
    Salidas: bono (unidades extra que recibe la comuna)
    */
    int bono = 0;

    bono = (cantidad * PORCENTAJE_BONO_TRUEQUE + 99) / 100;

    return bono;
}

void truequeBienes(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, char *nombre_buscado, int cantidad_buscada) {
    /*
    Funcionamiento: Permite a una comuna solicitar un recurso a otra comuna a cambio de otro recurso
    Entradas: comunaSolicitante (puntero a la comuna que solicita el recurso),
              comunaProveedora (puntero a la comuna que provee el recurso),
              nombre_buscado (nombre del recurso que se solicita),
              cantidad_buscada (cantidad del recurso que se solicita)
    Salidas: ninguna (actualiza directamente los bienes de ambas comunas)
    */
    struct Recursos *B_recurso_proveedora = NULL; // Puntero al recurso que la comuna proveedora tiene y que la comuna solicitante quiere

    struct Recursos *B_recurso_pedido = NULL; // Puntero al recurso con menor cantidad en la comuna proveedora, el que ella pedira a cambio

    struct Recursos *A_recurso_solicitante = NULL; // Puntero al recurso que la comuna solicitante tiene y que la comuna proveedora quiere

    struct Recursos *A_recurso_solicitante_recibe = NULL; // Puntero al recurso que la comuna solicitante recibirá de la comuna proveedora

    int B_personas_proveedora = 0; // Cantidad de personas en la comuna proveedora

    int A_personas_solicitante = 0; // Cantidad de personas en la comuna solicitante

    int cantidad_pedida = 0; // Cantidad del recurso que la comuna solicitante debe dar a cambio

    int extra = 0; // Cantidad extra que la comuna solicitante debe dar a cambio si hay un aumento aleatorio

    int puede_dar = 0; // Variable para verificar si la comuna proveedora puede dar el recurso sin quedar en riesgo

    int probabilidad_extra = 0; // Variable para determinar si hay un aumento aleatorio en la cantidad pedida

    int A_bono_solicitante = 0; // Variable para almacenar el bono que recibirá la comuna solicitante

    int B_bono_proveedora = 0; // Variable para almacenar el bono que recibirá la comuna proveedora

    int cantidad_final = 0; // Cantidad que tendria una comuna despues de recibir, para compararla con su maximo

    int respuesta = 0; // Respuesta del usuario: 1 = continuar, 2 = no continuar, 0 = opcion no valida

    // Validar que el trueque tenga sentido (comunas distintas y cantidad mayor que 0)
    if (validarTrueque(comunaSolicitante, comunaProveedora, cantidad_buscada) == 0) {
        return;
    }

    // Buscar el recurso solicitado en la comuna proveedora
    B_recurso_proveedora = buscarRecurso(comunaProveedora->bienes, nombre_buscado);
    if (B_recurso_proveedora == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene %s.\n", comunaProveedora->nombre, nombre_buscado);
        return;
    }

    // Verificar si la comuna proveedora puede dar el recurso sin quedar en riesgo
    B_personas_proveedora = recorrerPersonas(comunaProveedora->personas);
    puede_dar = puedeDarRecurso(B_recurso_proveedora, B_personas_proveedora, cantidad_buscada);
    if (puede_dar == 0) {
        printf("No se pudo hacer el trueque: %s no puede dar %s sin quedar en riesgo.\n", comunaProveedora->nombre, nombre_buscado);
        return;
    }

    // Buscar un recurso alternativo en la comuna proveedora que no sea el recurso solicitado
    B_recurso_pedido = encontrarRecursoMasBajo(comunaProveedora->bienes, nombre_buscado);
    if (B_recurso_pedido == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene otro bien para pedir a cambio.\n", comunaProveedora->nombre);
        return;
    }

    cantidad_pedida = cantidad_buscada; // La cantidad pedida es igual a la cantidad buscada inicialmente

    // Determinar si hay un aumento aleatorio en la cantidad pedida (25% de probabilidad)
    probabilidad_extra = rand() % 4;
    if (probabilidad_extra == 0) {
        extra = (rand() % 4) + 2;
        cantidad_pedida = cantidad_pedida + extra;
    }

    // Buscar el recurso que la comuna solicitante tiene y que la comuna proveedora quiere
    A_recurso_solicitante = buscarRecurso(comunaSolicitante->bienes, B_recurso_pedido->nombre);
    if (A_recurso_solicitante == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene %s para dar a cambio.\n", comunaSolicitante->nombre, B_recurso_pedido->nombre);
        return;
    }

    // Verificar si la comuna solicitante puede dar el recurso sin quedar en riesgo
    A_personas_solicitante = recorrerPersonas(comunaSolicitante->personas);
    puede_dar = puedeDarRecurso(A_recurso_solicitante, A_personas_solicitante, cantidad_pedida);
    if (puede_dar == 0) {
        printf("No se pudo hacer el trueque: %s no puede dar %d de %s a cambio.\n", comunaSolicitante->nombre, cantidad_pedida, B_recurso_pedido->nombre);
        return;
    }

    // Buscar el recurso que la comuna solicitante recibirá de la comuna proveedora
    A_recurso_solicitante_recibe = buscarRecurso(comunaSolicitante->bienes, B_recurso_proveedora->nombre);
    if (A_recurso_solicitante_recibe == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene %s para recibir.\n", comunaSolicitante->nombre, B_recurso_proveedora->nombre);
        return;
    }

    // Verificar que la comuna solicitante no pase de su cantidad maxima al recibir lo que pidio
    cantidad_final = A_recurso_solicitante_recibe->cantidad + cantidad_buscada;
    if (cantidad_final > A_recurso_solicitante_recibe->cantidadMaxima) {
        printf("No se pudo hacer el trueque: %s no puede recibir %d de %s porque pasaria su maximo.\n", comunaSolicitante->nombre, cantidad_buscada, nombre_buscado);
        return;
    }

    // Verificar que la comuna proveedora no pase de su cantidad maxima al recibir lo que pidio a cambio
    cantidad_final = B_recurso_pedido->cantidad + cantidad_pedida;
    if (cantidad_final > B_recurso_pedido->cantidadMaxima) {
        printf("No se pudo hacer el trueque: %s no puede recibir %d de %s porque pasaria su maximo.\n", comunaProveedora->nombre, cantidad_pedida, B_recurso_pedido->nombre);
        return;
    }

    // Calcular el bono de reciprocidad para cada comuna según la cantidad que recibe
    A_bono_solicitante = calcularBono(cantidad_buscada);
    B_bono_proveedora = calcularBono(cantidad_pedida);

    // Mostrar el intercambio al usuario y pedirle confirmacion antes de hacerlo
    printf("Propuesta de trueque de bienes:\n");
    printf("  %s recibe %d de %s de parte de %s.\n", comunaSolicitante->nombre, cantidad_buscada, nombre_buscado, comunaProveedora->nombre);
    printf("  %s recibe %d de %s de parte de %s.\n", comunaProveedora->nombre, cantidad_pedida, B_recurso_pedido->nombre, comunaSolicitante->nombre);
    printf("  Bono de reciprocidad: %s +%d y %s +%d.\n", comunaSolicitante->nombre, A_bono_solicitante, comunaProveedora->nombre, B_bono_proveedora);
    pedirConfirmacion(&respuesta);
    if (respuesta == 2) {
        printf(COLOR_AMARILLO "No se acepto el trueque.\n" COLOR_RESET);
        return;
    }
    if (respuesta != 1) {
        printf(COLOR_AMARILLO "Esa no es una opcion valida. Trueque cancelado.\n" COLOR_RESET);
        return;
    }

    // Actualizar las cantidades de los recursos en ambas comunas después del trueque
    B_recurso_proveedora->cantidad = B_recurso_proveedora->cantidad - cantidad_buscada;

    // Actualizar la cantidad del recurso que la comuna solicitante recibirá, asegurándose de no exceder la cantidad máxima
    A_recurso_solicitante_recibe->cantidad = A_recurso_solicitante_recibe->cantidad + cantidad_buscada + A_bono_solicitante;
    if (A_recurso_solicitante_recibe->cantidad > A_recurso_solicitante_recibe->cantidadMaxima) {
        A_recurso_solicitante_recibe->cantidad = A_recurso_solicitante_recibe->cantidadMaxima;
    }

    // Actualizar la cantidad del recurso que la comuna solicitante dará a cambio
    A_recurso_solicitante->cantidad = A_recurso_solicitante->cantidad - cantidad_pedida;

    // Actualizar la cantidad del recurso que la comuna proveedora recibirá, asegurándose de no exceder la cantidad máxima
    B_recurso_pedido->cantidad = B_recurso_pedido->cantidad + cantidad_pedida + B_bono_proveedora;
    if (B_recurso_pedido->cantidad > B_recurso_pedido->cantidadMaxima) {
        B_recurso_pedido->cantidad = B_recurso_pedido->cantidadMaxima;
    }

    printf(COLOR_VERDE "Trueque exitoso: %s le dio %d de %s a %s, y recibio %d de %s.\n" COLOR_RESET, comunaProveedora->nombre, cantidad_buscada, nombre_buscado, comunaSolicitante->nombre, cantidad_pedida, B_recurso_pedido->nombre);
}

void truequeServicios(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, char *nombre_buscado, int cantidad_buscada) {
    /*
    Funcionamiento: Permite a una comuna solicitar un servicio a otra comuna a cambio de otro servicio, siempre 1 a 1
    Entradas: comunaSolicitante (puntero a la comuna que solicita el servicio),
              comunaProveedora (puntero a la comuna que provee el servicio),
              nombre_buscado (nombre del servicio que se solicita),
              cantidad_buscada (cantidad del servicio que se solicita)
    Salidas: ninguna (actualiza directamente los servicios de ambas comunas)
    */
    struct Servicios *B_servicio_proveedora = NULL; // Puntero al servicio que la comuna proveedora tiene y que la comuna solicitante quiere

    struct Servicios *B_servicio_pedido = NULL; // Puntero al servicio con menor cantidad en la comuna proveedora, el que ella pedira a cambio

    struct Servicios *A_servicio_solicitante = NULL; // Puntero al servicio que la comuna solicitante tiene y que la comuna proveedora quiere

    struct Servicios *A_servicio_solicitante_recibe = NULL; // Puntero al servicio que la comuna solicitante recibirá de la comuna proveedora

    int B_personas_proveedora = 0; // Cantidad de personas en la comuna proveedora

    int A_personas_solicitante = 0; // Cantidad de personas en la comuna solicitante

    int B_personas_con_oficio = 0; // Cantidad de personas de la comuna proveedora que tienen el oficio que se va a dar

    int A_personas_con_oficio = 0; // Cantidad de personas de la comuna solicitante que tienen el oficio que se va a dar a cambio

    int puede_dar = 0; // Variable para verificar si una comuna puede dar el servicio sin quedar en emergencia

    int cantidad_final = 0; // Cantidad que tendría una comuna después de recibir, para compararla con su máximo

    int respuesta = 0; // Respuesta del usuario: 1 = continuar, 2 = no continuar, 0 = opcion no valida

    int bono = 0; // Bono de reciprocidad que recibe cada comuna (las dos reciben la misma cantidad porque el trueque es 1 a 1)

    // Validar que el trueque tenga sentido (comunas distintas y cantidad mayor que 0)
    if (validarTrueque(comunaSolicitante, comunaProveedora, cantidad_buscada) == 0) {
        return;
    }

    // Buscar el servicio solicitado en la comuna proveedora
    B_servicio_proveedora = buscarServicio(comunaProveedora->servicios, nombre_buscado);
    if (B_servicio_proveedora == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene %s.\n", comunaProveedora->nombre, nombre_buscado);
        return;
    }

    // Verificar si la comuna proveedora puede dar el servicio sin quedar en emergencia
    B_personas_proveedora = recorrerPersonas(comunaProveedora->personas);
    puede_dar = puedeDarServicio(comunaProveedora->servicios, B_servicio_proveedora, B_personas_proveedora, cantidad_buscada);
    if (puede_dar == 0) {
        printf("No se pudo hacer el trueque: %s no puede dar %s sin quedar en riesgo.\n", comunaProveedora->nombre, nombre_buscado);
        return;
    }

    // Buscar el servicio solicitado en la comuna solicitante, que es donde se va a recibir
    A_servicio_solicitante_recibe = buscarServicio(comunaSolicitante->servicios, nombre_buscado);
    if (A_servicio_solicitante_recibe == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene %s para recibir.\n", comunaSolicitante->nombre, nombre_buscado);
        return;
    }

    // Verificar que la comuna solicitante no pase de su cantidad máxima al recibir
    cantidad_final = A_servicio_solicitante_recibe->cantidad + cantidad_buscada;
    if (cantidad_final > A_servicio_solicitante_recibe->cantidadMaxima) {
        printf("No se pudo hacer el trueque: %s no puede recibir %d de %s porque pasaria su maximo.\n", comunaSolicitante->nombre, cantidad_buscada, nombre_buscado);
        return;
    }

    // Buscar un servicio alternativo en la comuna proveedora que no sea el servicio solicitado
    B_servicio_pedido = encontrarServicioMasBajo(comunaProveedora->servicios, nombre_buscado);
    if (B_servicio_pedido == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene otro servicio para pedir a cambio.\n", comunaProveedora->nombre);
        return;
    }

    // Verificar que la comuna proveedora no pase de su cantidad máxima al recibir el servicio pedido
    cantidad_final = B_servicio_pedido->cantidad + cantidad_buscada;
    if (cantidad_final > B_servicio_pedido->cantidadMaxima) {
        printf("No se pudo hacer el trueque: %s no puede recibir %d de %s porque pasaria su maximo.\n", comunaProveedora->nombre, cantidad_buscada, B_servicio_pedido->nombre);
        return;
    }

    // Buscar el servicio que la comuna solicitante tiene y que la comuna proveedora quiere
    A_servicio_solicitante = buscarServicio(comunaSolicitante->servicios, B_servicio_pedido->nombre);
    if (A_servicio_solicitante == NULL) {
        printf("No se pudo hacer el trueque: %s no tiene %s para dar a cambio.\n", comunaSolicitante->nombre, B_servicio_pedido->nombre);
        return;
    }

    // Verificar si la comuna solicitante puede dar el servicio sin quedar en emergencia
    A_personas_solicitante = recorrerPersonas(comunaSolicitante->personas);
    puede_dar = puedeDarServicio(comunaSolicitante->servicios, A_servicio_solicitante, A_personas_solicitante, cantidad_buscada);
    if (puede_dar == 0) {
        printf("No se pudo hacer el trueque: %s no puede dar %d de %s a cambio.\n", comunaSolicitante->nombre, cantidad_buscada, B_servicio_pedido->nombre);
        return;
    }

    // Verificar que la comuna proveedora tenga las personas con ese oficio para poder trasladarlas
    B_personas_con_oficio = contarPersonasPorOficio(comunaProveedora->personas, B_servicio_proveedora->oficio);
    if (B_personas_con_oficio < cantidad_buscada) {
        printf("No se pudo hacer el trueque: %s no tiene %d personas con el oficio de %s para trasladar.\n", comunaProveedora->nombre, cantidad_buscada, nombre_buscado);
        return;
    }

    // Verificar que la comuna solicitante tenga las personas con ese oficio para poder trasladarlas a cambio
    A_personas_con_oficio = contarPersonasPorOficio(comunaSolicitante->personas, A_servicio_solicitante->oficio);
    if (A_personas_con_oficio < cantidad_buscada) {
        printf("No se pudo hacer el trueque: %s no tiene %d personas con el oficio de %s para trasladar.\n", comunaSolicitante->nombre, cantidad_buscada, B_servicio_pedido->nombre);
        return;
    }

    // Calcular el bono de reciprocidad (el mismo para ambas comunas)
    bono = calcularBono(cantidad_buscada);

    // Mostrar el intercambio al usuario y pedirle confirmacion antes de hacerlo
    printf("Propuesta de trueque de servicios:\n");
    printf("  %s recibe %d de %s de parte de %s.\n", comunaSolicitante->nombre, cantidad_buscada, nombre_buscado, comunaProveedora->nombre);
    printf("  %s recibe %d de %s de parte de %s.\n", comunaProveedora->nombre, cantidad_buscada, B_servicio_pedido->nombre, comunaSolicitante->nombre);
    printf("  Bono de reciprocidad: cada comuna recibe +%d.\n", bono);
    pedirConfirmacion(&respuesta);
    if (respuesta == 2) {
        printf(COLOR_AMARILLO "No se acepto el trueque.\n" COLOR_RESET);
        return;
    }
    if (respuesta != 1) {
        printf(COLOR_AMARILLO "Esa no es una opcion valida. Trueque cancelado.\n" COLOR_RESET);
        return;
    }

    // Actualizar las cantidades de los servicios en ambas comunas después del trueque (1 a 1)
    B_servicio_proveedora->cantidad = B_servicio_proveedora->cantidad - cantidad_buscada;
    A_servicio_solicitante->cantidad = A_servicio_solicitante->cantidad - cantidad_buscada;

    // Cada comuna recibe lo que pidio mas el bono de reciprocidad, sin pasar de su cantidad maxima
    A_servicio_solicitante_recibe->cantidad = A_servicio_solicitante_recibe->cantidad + cantidad_buscada + bono;
    if (A_servicio_solicitante_recibe->cantidad > A_servicio_solicitante_recibe->cantidadMaxima) {
        A_servicio_solicitante_recibe->cantidad = A_servicio_solicitante_recibe->cantidadMaxima;
    }

    B_servicio_pedido->cantidad = B_servicio_pedido->cantidad + cantidad_buscada + bono;
    if (B_servicio_pedido->cantidad > B_servicio_pedido->cantidadMaxima) {
        B_servicio_pedido->cantidad = B_servicio_pedido->cantidadMaxima;
    }

    // Trasladar a las personas: las del oficio pedido pasan a la comuna solicitante y las del oficio a cambio pasan a la comuna proveedora
    trasladarPersonas(comunaProveedora, comunaSolicitante, B_servicio_proveedora->oficio, cantidad_buscada);
    trasladarPersonas(comunaSolicitante, comunaProveedora, A_servicio_solicitante->oficio, cantidad_buscada);

    printf(COLOR_VERDE "Trueque exitoso: %s le dio %d de %s a %s, y recibio %d de %s.\n" COLOR_RESET, comunaProveedora->nombre, cantidad_buscada, nombre_buscado, comunaSolicitante->nombre, cantidad_buscada, B_servicio_pedido->nombre);
}

void trueque(struct Comuna *comunaSolicitante, struct Comuna *comunaProveedora, int tipo, char *nombre_buscado, int cantidad_buscada) {
    /*
    Funcionamiento: Decide si el trueque es de servicios o de bienes y llama a la función correspondiente
    Entradas: comunaSolicitante (puntero a la comuna que solicita),
              comunaProveedora (puntero a la comuna que provee),
              tipo (1 para servicios, 2 para bienes),
              nombre_buscado (nombre del servicio o bien que se solicita),
              cantidad_buscada (cantidad que se solicita)
    Salidas: ninguna (actualiza directamente los servicios o bienes de ambas comunas)
    */
    if (tipo == 1) {
        truequeServicios(comunaSolicitante, comunaProveedora, nombre_buscado, cantidad_buscada);
    } else if (tipo == 2) {
        truequeBienes(comunaSolicitante, comunaProveedora, nombre_buscado, cantidad_buscada);
    } else {
        printf("Opcion no valida.\n");
    }
}

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
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 2, cantidadComunas)); // Vecina anterior
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas)); // Vecina anterior
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas)); // Vecina siguiente
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 2, cantidadComunas)); // Vecina siguiente
        cantidad_vecinas = 4;
    } else if (cantidadComunas == 4) { // Si hay 4 comunas, se consideran las 3 vecinas más cercanas
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 2, cantidadComunas)); // Vecina anterior
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas)); // Vecina anterior
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas)); // Vecina siguiente
        cantidad_vecinas = 3;
    } else if (cantidadComunas == 3) { // Si hay 3 comunas, se consideran las 2 vecinas más cercanas
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, cantidadComunas - 1, cantidadComunas)); // Vecina anterior
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas)); // Vecina siguiente
        cantidad_vecinas = 2;
    } else if (cantidadComunas == 2) { // Si hay 2 comunas, se considera la única vecina
        suma_vecinas = suma_vecinas + formulaSatisfaccion(avanzarComunas(actual, 1, cantidadComunas)); // Vecina siguiente
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
    Funcionamiento: recorre todas las comunas de la lista circular y les actualiza su necesidad y satisfaccion usando las formulas.
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
    Funcionamiento: reduce la cantidad de cada bien de la comuna segun lo que consumieron sus personas en el turno.
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

void generarEmergencia(struct Comuna *actual, int cantidadComunas) {
    /*
    Funcionamiento: genera una emergencia aleatoria en la comuna actual y, si es grave, también afecta a las comunas vecinas
    Entradas: actual (puntero a la comuna donde ocurre la emergencia),
              cantidadComunas (número total de comunas en la lista circular)
    Salidas: ninguna (actualiza directamente los bienes o servicios de la comuna y sus vecinas)
    */
    int tipo_emergencia = 0;
    int perdida = 0;
    char *nombre_emergencia = "";
    int grave = 0;
    struct Comuna *siguiente = NULL;
    struct Comuna *anterior = NULL;

    tipo_emergencia = (rand() % 5) + 1; // se selecciona un tipo de emergencia aleatorio entre 1 y 5

    if (tipo_emergencia == 1) {
        nombre_emergencia = "un incendio";
        perdida = (rand() % 5) + 1;
        grave = 0;
    } else if (tipo_emergencia == 2) {
        nombre_emergencia = "una inundacion";
        perdida = (rand() % 5) + 2;
        grave = 0;
    } else if (tipo_emergencia == 3) {
        nombre_emergencia = "una sequia";
        perdida = (rand() % 4) + 3;
        grave = 0;
    } else if (tipo_emergencia == 4) {
        nombre_emergencia = "un terremoto";
        perdida = (rand() % 5) + 3;
        grave = 1;
    } else {
        nombre_emergencia = "una revolucion";
        perdida = (rand() % 5) + 4;
        grave = 1;
    }

    // Aplicar la emergencia a la comuna actual
    alcanceEmergencia(actual, perdida, nombre_emergencia);

    // Si la emergencia es grave, también se aplica a las comunas vecinas
    if (grave == 1 && cantidadComunas >= 2) {
        siguiente = avanzarComunas(actual, 1, cantidadComunas);
        alcanceEmergencia(siguiente, perdida, nombre_emergencia);

        // Si hay al menos 3 comunas, también se aplica a la comuna anterior
        if (cantidadComunas >= 3) {
            anterior = avanzarComunas(actual, cantidadComunas - 1, cantidadComunas);
            alcanceEmergencia(anterior, perdida, nombre_emergencia);
        }
    }
}

void controlarEmergencias(struct Comuna *inicio, int cantidadComunas, int *turnosHastaEmergencia) {
    /*
    Funcionamiento: genera una emergencia aleatoria en una comuna aleatoria, y si es grave, también afecta a las comunas vecinas. 
    Entradas: inicio (puntero a cualquier comuna de la lista circular), 
              cantidadComunas (número total de comunas en la lista circular), 
              turnosHastaEmergencia (puntero a un entero que indica cuántos turnos faltan para que ocurra la próxima emergencia)
    Salidas: ninguna (actualiza directamente los bienes o servicios de la comuna y sus vecinas, y actualiza el valor de turnosHastaEmergencia)
    */
    int indiceComuna = 0;
    struct Comuna *actual = NULL;

    *turnosHastaEmergencia = *turnosHastaEmergencia - 1; // Disminuir el contador de turnos hasta la próxima emergencia

    if (*turnosHastaEmergencia > 0) { //
        return;
    }

    indiceComuna = rand() % cantidadComunas; // Seleccionar una comuna aleatoria para la emergencia

    actual = inicio;
    // Avanzar hasta la comuna seleccionada
    for (int i = 0; i < indiceComuna; i++) {
        actual = actual->siguiente;
    }

    generarEmergencia(actual, cantidadComunas);

    *turnosHastaEmergencia = (rand() % 4) + 4; // Reiniciar el contador de turnos hasta la próxima emergencia (entre 4 y 7 turnos)
}

void pasarTurno(struct Comuna *inicio, int cantidadComunas, int *turnosHastaEmergencia) {
    /*
    Funcionamiento: aplica el consumo de bienes a todas las comunas, luego actualiza sus indices de necesidad y satisfaccion y finalmente controla si ocurre una emergencia en alguna comuna.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas en la lista)
    Salidas: ninguna
    */
    aplicarConsumoTodas(inicio);
    actualizarIndices(inicio, cantidadComunas);
    controlarEmergencias(inicio, cantidadComunas, turnosHastaEmergencia);
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

    float bono_flotante = 0; // Variable para calcular el bono flotante que se dará a la comuna solicitante y proveedora

    int A_bono_solicitante = 0; // Variable para almacenar el bono que recibirá la comuna solicitante

    int B_bono_proveedora = 0; // Variable para almacenar el bono que recibirá la comuna proveedora

    int cantidad_final = 0; // Cantidad que tendria una comuna despues de recibir, para compararla con su maximo

    int respuesta = 0; // Respuesta del usuario: 1 = continuar, 2 = no continuar, 0 = opcion no valida

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

    // Calcular el bono para la comuna solicitante según la cantidad de recursos intercambiados
    bono_flotante = 0.3 * cantidad_buscada;
    A_bono_solicitante = (int) bono_flotante;
    if (bono_flotante > A_bono_solicitante) {
        A_bono_solicitante = A_bono_solicitante + 1;
    }

    // Calcular el bono para la comuna proveedora según la cantidad de recursos intercambiados
    bono_flotante = 0.3 * cantidad_pedida;
    B_bono_proveedora = (int) bono_flotante;
    if (bono_flotante > B_bono_proveedora) {
        B_bono_proveedora = B_bono_proveedora + 1;
    }

    // Mostrar el intercambio al usuario y pedirle confirmacion antes de hacerlo
    printf("Propuesta de trueque de bienes:\n");
    printf("  %s recibe %d de %s de parte de %s.\n", comunaSolicitante->nombre, cantidad_buscada, nombre_buscado, comunaProveedora->nombre);
    printf("  %s recibe %d de %s de parte de %s.\n", comunaProveedora->nombre, cantidad_pedida, B_recurso_pedido->nombre, comunaSolicitante->nombre);
    pedirConfirmacion(&respuesta);
    if (respuesta == 2) {
        printf("No se acepto el trueque.\n");
        return;
    }
    if (respuesta != 1) {
        printf("Esa no es una opcion valida. Trueque cancelado.\n");
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

    printf("Trueque exitoso: %s le dio %d de %s a %s, y recibio %d de %s.\n", comunaProveedora->nombre, cantidad_buscada, nombre_buscado, comunaSolicitante->nombre, cantidad_pedida, B_recurso_pedido->nombre);
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

    // Mostrar el intercambio al usuario y pedirle confirmacion antes de hacerlo
    printf("Propuesta de trueque de servicios:\n");
    printf("  %s recibe %d de %s de parte de %s.\n", comunaSolicitante->nombre, cantidad_buscada, nombre_buscado, comunaProveedora->nombre);
    printf("  %s recibe %d de %s de parte de %s.\n", comunaProveedora->nombre, cantidad_buscada, B_servicio_pedido->nombre, comunaSolicitante->nombre);
    pedirConfirmacion(&respuesta);
    if (respuesta == 2) {
        printf("No se acepto el trueque.\n");
        return;
    }
    if (respuesta != 1) {
        printf("Esa no es una opcion valida. Trueque cancelado.\n");
        return;
    }

    // Actualizar las cantidades de los servicios en ambas comunas después del trueque (1 a 1)
    B_servicio_proveedora->cantidad = B_servicio_proveedora->cantidad - cantidad_buscada;
    A_servicio_solicitante_recibe->cantidad = A_servicio_solicitante_recibe->cantidad + cantidad_buscada;
    A_servicio_solicitante->cantidad = A_servicio_solicitante->cantidad - cantidad_buscada;
    B_servicio_pedido->cantidad = B_servicio_pedido->cantidad + cantidad_buscada;

    // Trasladar a las personas: las del oficio pedido pasan a la comuna solicitante y las del oficio a cambio pasan a la comuna proveedora
    trasladarPersonas(comunaProveedora, comunaSolicitante, B_servicio_proveedora->oficio, cantidad_buscada);
    trasladarPersonas(comunaSolicitante, comunaProveedora, A_servicio_solicitante->oficio, cantidad_buscada);

    printf("Trueque exitoso: %s le dio %d de %s a %s, y recibio %d de %s.\n", comunaProveedora->nombre, cantidad_buscada, nombre_buscado, comunaSolicitante->nombre, cantidad_buscada, B_servicio_pedido->nombre);
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
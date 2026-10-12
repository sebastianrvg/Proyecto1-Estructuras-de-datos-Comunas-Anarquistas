#include <stdio.h>
#include <stdlib.h>
#include "Emergencias.h"
#include "Busqueda.h"
#include "Colores.h"

void alcanceEmergencia(struct Comuna *comuna, int perdida, char *nombre_emergencia) {
    /*
    Funcionamiento: Aplica el efecto de una emergencia a un recurso o servicio aleatorio de la comuna
    Entradas: comuna (puntero a la comuna afectada),
    perdida (gravedad de la emergencia; cada punto quita un porcentaje de la cantidad maxima del bien o servicio afectado),
            nombre_emergencia (nombre de la emergencia)
    Salidas: ninguna (actualiza directamente los bienes o servicios de la comuna)
    */
    int bien_seleccionado = 0;
    int cantidadNodos = 0;
    int indice = 0;
    int perdida_real = 0; // Unidades que realmente se pierden segun la cantidad maxima del bien o servicio afectado

    // Seleccionar aleatoriamente si la emergencia afectará un recurso o un servicio
    bien_seleccionado = rand() % 2;

    // si se selecciona un recurso, contar la cantidad de recursos en la comuna
    if (bien_seleccionado == 1) {
        struct Recursos *actual = comuna->bienes;

        while (actual != NULL) {
            cantidadNodos = cantidadNodos + 1;
            actual = actual->siguiente;
        }

        if (cantidadNodos == 0) {
            return;
        }

        indice = rand() % cantidadNodos;
        actual = comuna->bienes;
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        // Calcular las unidades perdidas como porcentaje de la cantidad maxima (minimo 1 unidad)
        perdida_real = (actual->cantidadMaxima * perdida * PORCENTAJE_PERDIDA_POR_PUNTO) / 100;
        if (perdida_real < 1) {
            perdida_real = 1;
        }

        if (actual->emergencias == 1) {
            printf(COLOR_ROJO "Ha ocurrido %s en %s. El bien %s ya estaba en emergencia y perdio %d unidades mas.\n" COLOR_RESET, nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        } else {
            printf(COLOR_ROJO "Ha ocurrido %s en %s. Se vio afectado el bien %s, se perdieron %d unidades.\n" COLOR_RESET, nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        }

        actual->cantidad = actual->cantidad - perdida_real;
        if (actual->cantidad < 0) {
            actual->cantidad = 0;
        }

    // Si lo que salio fue un servicio
    } else {
        struct Servicios *actual = comuna->servicios; // Puntero para recorrer la lista de servicios

        // Contar la cantidad de servicios en la comuna
        while (actual != NULL) {
            cantidadNodos = cantidadNodos + 1;
            actual = actual->siguiente;
        }

        // Si no hay servicios, no se puede aplicar la emergencia
        if (cantidadNodos == 0) {
            return;
        }

        indice = rand() % cantidadNodos; // Seleccionar un índice aleatorio para el servicio afectado
        actual = comuna->servicios; // Puntero para recorrer la lista de servicios nuevamente

        // Avanzar hasta el servicio seleccionado
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        // Calcular las unidades perdidas como porcentaje de la cantidad maxima (minimo 1 unidad)
        perdida_real = (actual->cantidadMaxima * perdida * PORCENTAJE_PERDIDA_POR_PUNTO) / 100;
        if (perdida_real < 1) {
            perdida_real = 1;
        }

        // Si el servicio ya estaba en emergencia, informar que perdió más unidades
        if (actual->emergencias == 1) {
            printf(COLOR_ROJO "Ha ocurrido %s en %s. El servicio %s ya estaba en emergencia y perdio %d unidades mas.\n" COLOR_RESET, nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);

        // Si el servicio no estaba en emergencia, informar que se vio afectado y perdió unidades
        } else {
            printf(COLOR_ROJO "Ha ocurrido %s en %s. Se vio afectado el servicio %s, se perdieron %d unidades.\n" COLOR_RESET, nombre_emergencia, comuna->nombre, actual->nombre, perdida_real);
        }

        // Actualizar la cantidad del servicio afectado por la emergencia
        actual->cantidad = actual->cantidad - perdida_real; // Reducir la cantidad del servicio afectado por la emergencia

        // Asegurarse de que la cantidad del servicio no sea negativa
        if (actual->cantidad < 0) {
            actual->cantidad = 0;
        }
    }
}

int hayRegalo(void) {
    /*
    Funcionamiento: Decide al azar si una emergencia va a recibir un regalo de otra comuna, con la probabilidad PROBABILIDAD_REGALO (en porcentaje)
    Entradas: ninguna
    Salidas: 1 si hay regalo, 0 si no
    */
    int tirada = 0; // Numero al azar entre 0 y 99

    tirada = rand() % 100;
    if (tirada < PROBABILIDAD_REGALO) {
        return 1;
    }

    return 0;
}

int regalarRecursos(struct Comuna *comunaAfectada, int regalo) {
    /*
    Funcionamiento: Recorre las demas comunas una por una, empezando por la siguiente a la afectada, y la primera que pueda regalar sin quedar en emergencia le pasa la cantidad indicada a la comuna afectada, sin pedir nada a cambio. El recurso regalado es el que tiene menos cantidad en la comuna afectada. Solo se regalan recursos, nunca servicios
    Entradas: comunaAfectada (puntero a la comuna que recibe el regalo),
              regalo (cantidad que se regala)
    Salidas: 1 si se hizo el regalo (e imprime un mensaje), 0 si no se pudo (no imprime nada)
    */
    struct Recursos *receptor_recurso = NULL; // Recurso de la comuna afectada que recibe el regalo (el que tiene menos cantidad)
    struct Recursos *donante_recurso = NULL; // Mismo recurso, pero en la comuna que regala
    struct Comuna *donante = NULL; // Comuna que se esta revisando para ver si puede regalar
    int personas_donante = 0; // Cantidad de personas de la comuna que se esta revisando
    int puede_dar = 0; // Indica si la comuna que se esta revisando puede regalar sin quedar en riesgo
    int cantidad_final = 0; // Cantidad que tendria la comuna afectada despues de recibir, para compararla con su maximo

    if (comunaAfectada == NULL || regalo <= 0) {
        return 0;
    }

    // Buscar el recurso con menos cantidad en la comuna afectada (el nombre vacio no excluye ninguno)
    receptor_recurso = encontrarRecursoMasBajo(comunaAfectada->bienes, "");
    if (receptor_recurso == NULL) {
        return 0;
    }

    // Verificar que la comuna afectada no pase de su cantidad maxima al recibir el regalo
    cantidad_final = receptor_recurso->cantidad + regalo;
    if (cantidad_final > receptor_recurso->cantidadMaxima) {
        return 0;
    }

    // Recorrer las demas comunas hasta encontrar una que pueda regalar
    donante = comunaAfectada->siguiente;
    while (donante != comunaAfectada) {
        donante_recurso = buscarRecurso(donante->bienes, receptor_recurso->nombre);
        if (donante_recurso != NULL) {
            personas_donante = recorrerPersonas(donante->personas);
            puede_dar = puedeDarRecurso(donante_recurso, personas_donante, regalo);
            if (puede_dar == 1) {
                donante_recurso->cantidad = donante_recurso->cantidad - regalo;
                receptor_recurso->cantidad = receptor_recurso->cantidad + regalo;
                printf(COLOR_VERDE "%s le regalo %d de %s a %s, sin pedir nada a cambio.\n" COLOR_RESET, donante->nombre, regalo, receptor_recurso->nombre, comunaAfectada->nombre);
                return 1;
            }
        }
        donante = donante->siguiente;
    }

    return 0;
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
    int hay_regalo = 0; // 1 si otra comuna va a intentar regalar recursos a la comuna afectada
    int regalo = 0; // Cantidad que se va a regalar
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

    // Anunciar cual emergencia ocurrio y a cuales comunas afecta (las graves tambien afectan a las vecinas)
    if (grave == 1 && cantidadComunas >= 3) {
        siguiente = avanzarComunas(actual, 1, cantidadComunas);
        anterior = avanzarComunas(actual, cantidadComunas - 1, cantidadComunas);
        printf(COLOR_ROJO "EMERGENCIA: %s afecta a %s, %s y %s.\n" COLOR_RESET, nombre_emergencia, actual->nombre, siguiente->nombre, anterior->nombre);
    } else if (grave == 1 && cantidadComunas == 2) {
        siguiente = avanzarComunas(actual, 1, cantidadComunas);
        printf(COLOR_ROJO "EMERGENCIA: %s afecta a %s y %s.\n" COLOR_RESET, nombre_emergencia, actual->nombre, siguiente->nombre);
    } else {
        printf(COLOR_ROJO "EMERGENCIA: %s afecta a %s.\n" COLOR_RESET, nombre_emergencia, actual->nombre);
    }

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

    // Con cierta probabilidad otra comuna le regala recursos a la comuna afectada (si ninguna puede, no se avisa nada)
    hay_regalo = hayRegalo();
    if (hay_regalo == 1) {
        regalo = (rand() % (REGALO_MAXIMO - REGALO_MINIMO + 1)) + REGALO_MINIMO;
        regalarRecursos(actual, regalo);
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

    // Si no hay comunas no se puede generar ninguna emergencia (evita dividir entre 0 en rand() % cantidadComunas)
    if (cantidadComunas <= 0) {
        return;
    }

    *turnosHastaEmergencia = *turnosHastaEmergencia - 1; // Disminuir el contador de turnos hasta la próxima emergencia

    if (*turnosHastaEmergencia > 0) {
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

void mostrarEmergencias(struct Comuna *inicio) {
    /*
    Funcionamiento: recorre la lista circular de comunas y muestra cuales tienen bienes
    o servicios en emergencia, junto con su cantidad actual y su cantidad maxima.
    Usa los indicadores de emergencia que calcula actualizarIndices, por lo que
    conviene llamarla despues de esa funcion.
    Entradas: inicio (puntero a cualquier comuna de la lista circular)
    Salidas: ninguna (imprime en pantalla)
    */
    struct Comuna *actual = inicio;
    struct Recursos *r = NULL;
    struct Servicios *s = NULL;
    int emergencias_comuna = 0;
    int comunas_en_emergencia = 0;

    if (inicio == NULL) {
        return;
    }

    printf("Comunas en emergencia:\n");

    while (1) {
        // Contar cuantas emergencias tiene esta comuna
        emergencias_comuna = 0;

        r = actual->bienes;
        while (r != NULL) {
            if (r->emergencias == 1) {
                emergencias_comuna = emergencias_comuna + 1;
            }
            r = r->siguiente;
        }

        s = actual->servicios;
        while (s != NULL) {
            if (s->emergencias == 1) {
                emergencias_comuna = emergencias_comuna + 1;
            }
            s = s->siguiente;
        }

        // Si tiene alguna, mostrar el detalle
        if (emergencias_comuna > 0) {
            comunas_en_emergencia = comunas_en_emergencia + 1;
            printf("%s:\n", actual->nombre);

            r = actual->bienes;
            while (r != NULL) {
                if (r->emergencias == 1) {
                    printf("  - Bien %s: %d de %d\n", r->nombre, r->cantidad, r->cantidadMaxima);
                }
                r = r->siguiente;
            }

            s = actual->servicios;
            while (s != NULL) {
                if (s->emergencias == 1) {
                    printf("  - Servicio %s: %d de %d\n", s->nombre, s->cantidad, s->cantidadMaxima);
                }
                s = s->siguiente;
            }
        }

        actual = actual->siguiente;
        if (actual == inicio) {
            break;
        }
    }

    if (comunas_en_emergencia == 0) {
        printf("Ninguna comuna esta en emergencia.\n");
    }
}
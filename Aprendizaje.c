#include <stdio.h>
#include <stdlib.h>
#include "Aprendizaje.h"
#include "Colores.h"

struct Servicios *elegirServicioNuevo(struct Persona *persona, struct Servicios *servicios) {
    /*
    Funcionamiento: elige al azar uno de los servicios de la comuna que la persona todavia no tiene (distinto de su oficio)
    Entradas: persona (persona que va a aprender), servicios (lista de servicios de su comuna)
    Salidas: el servicio elegido, o NULL si no hay ninguno que pueda aprender
    */
    struct Servicios *actual = servicios;
    int posibles = 0; // Servicios que la persona todavia no tiene
    int elegido = 0; // Posicion al azar entre los posibles

    while (actual != NULL) {
        if (actual->oficio != persona->oficio) {
            posibles = posibles + 1;
        }
        actual = actual->siguiente;
    }

    if (posibles == 0) {
        return NULL;
    }

    elegido = rand() % posibles;
    actual = servicios;
    while (actual != NULL) {
        if (actual->oficio != persona->oficio) {
            if (elegido == 0) {
                return actual;
            }
            elegido = elegido - 1;
        }
        actual = actual->siguiente;
    }

    return NULL;
}

int aprenderServicio(struct Persona *persona, struct Servicios *servicios) {
    /*
    Funcionamiento: la persona aprende un servicio nuevo al azar. Si no tenia oficio, ese pasa a ser su oficio; si ya tenia, queda como su servicio aprendido. El servicio de la comuna gana una persona mas que lo puede ofrecer
    Entradas: persona (persona que aprende), servicios (lista de servicios de su comuna)
    Salidas: 1 si aprendio un servicio, 0 si no habia ninguno nuevo para aprender
    */
    struct Servicios *nuevo = elegirServicioNuevo(persona, servicios);

    if (nuevo == NULL) {
        return 0;
    }

    if (persona->oficio == 0) {
        persona->oficio = nuevo->oficio;
    } else {
        persona->oficioAprendido = nuevo->oficio;
    }

    nuevo->cantidad = nuevo->cantidad + 1;
    nuevo->cantidadMaxima = nuevo->cantidadMaxima + 1;

    return 1;
}

void leerEnBiblioteca(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: cada dia, cada persona tiene una probabilidad de ir a la biblioteca y leer un libro. Al llegar a 3 libros leidos aprende un servicio nuevo, si todavia no ha aprendido ninguno. Imprime una linea por cada comuna donde alguien aprendio
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (actualiza las personas y los servicios de cada comuna)
    */
    struct Comuna *comuna = inicio;
    struct Persona *persona = NULL;
    int aprendieron = 0; // Personas que aprendieron un servicio hoy en la comuna

    for (int i = 0; i < cantidadComunas; i++) {
        aprendieron = 0;
        persona = comuna->personas;

        while (persona != NULL) {
            // Solo leen las personas que todavia no aprendieron un servicio
            if (persona->oficioAprendido == 0 && rand() % 100 < PROBABILIDAD_LECTURA) {
                persona->librosLeidos = persona->librosLeidos + 1;
                if (persona->librosLeidos >= LIBROS_PARA_APRENDER) {
                    persona->librosLeidos = 0;
                    aprendieron = aprendieron + aprenderServicio(persona, comuna->servicios);
                }
            }
            persona = persona->siguiente;
        }

        if (aprendieron > 0) {
            printf(COLOR_CYAN "En %s, %d persona(s) aprendieron un servicio nuevo en la biblioteca.\n" COLOR_RESET, comuna->nombre, aprendieron);
        }
        comuna = comuna->siguiente;
    }
}
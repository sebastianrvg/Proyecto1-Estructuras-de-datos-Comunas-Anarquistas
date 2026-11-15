#include <stdio.h>
#include <stdlib.h>
#include "Aprendizaje.h"
#include "Colores.h"
#include "Busqueda.h"

struct Servicios *elegirServicioNuevo(struct Servicios *servicios) {
    /*
    Funcionamiento: elige al azar uno de los servicios de la comuna, todos con la misma probabilidad
    Entradas: servicios (lista de servicios de la comuna)
    Salidas: el servicio elegido, o NULL si la comuna no tiene servicios
    */
    struct Servicios *actual = servicios;
    int elegido = 0; // Posicion al azar dentro de la lista

    if (servicios == NULL) {
        return NULL;
    }

    elegido = rand() % recorrerServicios(servicios);
    for (int i = 0; i < elegido; i++) {
        actual = actual->siguiente;
    }

    return actual;
}

int aprenderServicio(struct Persona *persona, struct Servicios *servicios) {
    /*
    Funcionamiento: una persona sin oficio aprende un servicio al azar, que pasa a ser su oficio
    Entradas: persona (persona que aprende), servicios (lista de servicios de su comuna)
    Salidas: 1 si aprendio un servicio, 0 si la comuna no tiene servicios
    */
    struct Servicios *nuevo = elegirServicioNuevo(servicios);

    if (nuevo == NULL) {
        return 0;
    }

    persona->oficio = nuevo->oficio;

    return 1;
}

void leerEnBiblioteca(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: cada dia, cada persona sin oficio tiene una probabilidad de ir a la biblioteca y leer un libro. Al llegar a 3 libros leidos aprende un servicio, que pasa a ser su oficio, y la comuna recalcula cuantas personas ofrecen cada servicio. Imprime una linea por cada comuna donde alguien aprendio
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
            // Solo van a la biblioteca las personas que todavia no tienen oficio
            if (persona->oficio == 0 && rand() % 100 < PROBABILIDAD_LECTURA) {
                persona->librosLeidos = persona->librosLeidos + 1;
                if (persona->librosLeidos >= LIBROS_PARA_APRENDER) {
                    persona->librosLeidos = 0;
                    aprendieron = aprendieron + aprenderServicio(persona, comuna->servicios);
                }
            }
            persona = persona->siguiente;
        }

        if (aprendieron > 0) {
            actualizarCantidadServicios(comuna);
            printf(COLOR_CYAN "En %s, %d persona(s) aprendieron un servicio nuevo en la biblioteca.\n" COLOR_RESET, comuna->nombre, aprendieron);
        }
        comuna = comuna->siguiente;
    }
}
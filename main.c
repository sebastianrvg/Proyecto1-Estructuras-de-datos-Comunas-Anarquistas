#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Nodos.h"
#include "Indices.h"
#include "Trueque.h"
#include "PasarDia.h"
#include "Emergencias.h"
#include "Sociedad.h"
#include "Colores.h"
#include "Busquedatexto.h"

void mostrarLogo(void) {
    /*
    Funcionamiento: imprime el logo del proyecto en ASCII al iniciar el programa.
    Entradas: ninguna
    Salidas: ninguna (imprime en pantalla)
    */
    printf(COLOR_MAGENTA);
    printf("=========================================================\n");
    printf("   C O M U N A S   A N A R Q U I S T A S\n");
    printf("   la economia del regalo y el trueque\n");
    printf("=========================================================\n");
    printf(COLOR_RESET);
}

void elegirComunasAdministradas(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: muestra las comunas y le pide al usuario que elija 3 distintas
    para administrar. Las demas quedan como autonomas.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (marca administrado = 1 en las comunas elegidas)
    */
    struct Comuna *actual = inicio;
    struct Comuna *elegida = NULL;
    char error[100];
    int numero = 0;
    int elegidas = 0;

    printf("Elija las 3 comunas que va a administrar. Las demas se manejan de forma autonoma\n");
    for (int i = 1; i <= cantidadComunas; i++) {
        printf("%d. %s\n", i, actual->nombre);
        actual = actual->siguiente;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    while (elegidas < 3) {
        printf("Comuna que administra (%d de 3): ", elegidas + 1);
        if (scanf("%d", &numero) != 1) {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
            scanf("%99s", error);
            continue;
        }
        if (numero < 1 || numero > cantidadComunas) {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
            continue;
        }
        elegida = avanzarComunas(inicio, numero - 1, cantidadComunas);
        if (elegida->administrado == 1) {
            printf(COLOR_AMARILLO "Esa comuna ya fue elegida.\n" COLOR_RESET);
            continue;
        }
        elegida->administrado = 1;
        elegidas = elegidas + 1;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
}

struct Comuna *siguiente_administrado(struct Comuna *actual) {
    /*
    Funcionamiento: avanza por la lista circular, ignorando las comunas que no se administran
    Entradas: actual (comuna desde donde se empieza a avanzar)
    Salidas: puntero a la siguiente comuna administrada
    */
    struct Comuna *siguiente = actual->siguiente;

    while (siguiente->administrado == 0) {
        siguiente = siguiente->siguiente;
    }

    return siguiente;
}

void menuTrueque(struct Comuna *inicio, int cantidadComunas, struct Comuna *solicitante) {
    /*
    Funcionamiento: guia al usuario para hacer un trueque: la comuna que solicita es la comuna actual, elige la que provee, si es de servicios o bienes, que pide y cuanto. Luego llama a trueque y actualiza los indices.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas),
              solicitante (comuna actual, una de las que el usuario administra)
    Salidas: ninguna (si algun dato no es valido, cancela el trueque)
    */
    struct Comuna *actual = inicio;
    struct Comuna *proveedora = NULL;
    struct Servicios *servicio = NULL;
    struct Recursos *recurso = NULL;
    char nombre_item[50]; // Nombre del servicio o bien que se pide
    char error[100];
    int numero_proveedora = 0;
    int tipo = 0; // 1 = servicios, 2 = bienes
    int opcion = 0; // Numero del servicio o bien elegido
    int cantidad = 0; // Cantidad que se pide

    if (cantidadComunas < 2) {
        printf(COLOR_AMARILLO "Hacen falta al menos 2 comunas para hacer un trueque.\n" COLOR_RESET);
        return;
    }

    for (int i = 1; i <= cantidadComunas; i++) {
        if (actual->administrado == 1) {
            printf("%d. %s (administrada)\n", i, actual->nombre);
        } else {
            printf("%d. %s\n", i, actual->nombre);
        }
        actual = actual->siguiente;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    printf("Solicita la comuna actual: %s\n", solicitante->nombre);
    printf("Numero de la comuna que provee: ");
    if (scanf("%d", &numero_proveedora) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    if (numero_proveedora < 1 || numero_proveedora > cantidadComunas) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        return;
    }
    proveedora = avanzarComunas(inicio, numero_proveedora - 1, cantidadComunas);

    printf("Tipo de trueque (1 = servicios, 2 = bienes): ");
    if (scanf("%d", &tipo) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    if (tipo == 1) {
        servicio = proveedora->servicios;
        opcion = 1;
        while (servicio != NULL) {
            printf("%d. %s (disponible: %d)\n", opcion, servicio->nombre, servicio->cantidad);
            opcion = opcion + 1;
            servicio = servicio->siguiente;
        }
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("Numero del servicio que solicita: ");
    } else if (tipo == 2) {
        recurso = proveedora->bienes;
        opcion = 1;
        while (recurso != NULL) {
            printf("%d. %s (disponible: %d)\n", opcion, recurso->nombre, recurso->cantidad);
            opcion = opcion + 1;
            recurso = recurso->siguiente;
        }
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("Numero del bien que solicita: ");
    } else {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        return;
    }

    if (scanf("%d", &opcion) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    // Buscar el nombre del servicio o bien elegido
    if (tipo == 1) {
        servicio = proveedora->servicios;
        for (int i = 1; i < opcion && servicio != NULL; i++) {
            servicio = servicio->siguiente;
        }
        if (opcion < 1 || servicio == NULL) {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
            return;
        }
        strcpy(nombre_item, servicio->nombre);
    } else {
        recurso = proveedora->bienes;
        for (int i = 1; i < opcion && recurso != NULL; i++) {
            recurso = recurso->siguiente;
        }
        if (opcion < 1 || recurso == NULL) {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
            return;
        }
        strcpy(nombre_item, recurso->nombre);
    }

    printf("Cantidad que solicita: ");
    if (scanf("%d", &cantidad) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    trueque(solicitante, proveedora, tipo, nombre_item, cantidad);
    actualizarIndices(inicio, cantidadComunas);
}

void menuDetalleComuna(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: pide una comuna y si se quieren ver sus bienes o sus servicios, y muestra la cantidad actual de cada uno junto con su maximo.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (imprime en pantalla; si algun dato no es valido, cancela)
    */
    struct Comuna *actual = inicio;
    struct Comuna *elegida = NULL;
    struct Recursos *recurso = NULL;
    struct Servicios *servicio = NULL;
    char error[100];
    int numero = 0;
    int tipo = 0; // 1 = servicios, 2 = bienes

    for (int i = 1; i <= cantidadComunas; i++) {
        printf("%d. %s\n", i, actual->nombre);
        actual = actual->siguiente;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    printf("Numero de la comuna que desea ver: ");
    if (scanf("%d", &numero) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    if (numero < 1 || numero > cantidadComunas) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        return;
    }
    elegida = avanzarComunas(inicio, numero - 1, cantidadComunas);

    printf("Que desea ver (1 = servicios, 2 = bienes): ");
    if (scanf("%d", &tipo) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("Regalos de %s: dados %d | recibidos %d\n", elegida->nombre, elegida->regalosDados, elegida->regalosRecibidos);
    printf("Reciprocidad de %s: %.2f\n", elegida->nombre, calcularReciprocidad(elegida));

    if (tipo == 1) {
        printf("Servicios de %s (personas disponibles de maximo)\n", elegida->nombre);
        servicio = elegida->servicios;
        while (servicio != NULL) {
            printf("  %s: %d de %d\n", servicio->nombre, servicio->cantidad, servicio->cantidadMaxima);
            servicio = servicio->siguiente;
        }
    } else if (tipo == 2) {
        printf("Bienes de %s (cantidad actual de maximo)\n", elegida->nombre);
        recurso = elegida->bienes;
        while (recurso != NULL) {
            printf("  %s: %d de %d\n", recurso->nombre, recurso->cantidad, recurso->cantidadMaxima);
            recurso = recurso->siguiente;
        }
    } else {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
    }
}

int main() {
    struct Comuna *inicio = NULL;
    struct Comuna *comuna_actual = NULL; // Comuna administrada con la que se trabaja
    struct NombreLibro *libros = NULL; // Libros de la biblioteca
    struct NodoChaflaneado *cache = NULL; // Arbol chaflaneado con las busquedas de texto ya hechas
    struct NombreLibro *resultado = NULL; // Libros que contienen el texto buscado (la lista es del arbol)
    char texto[100]; // Texto que se busca en los libros
    int i = -1;
    int cantidad_comunas = 0;
    int turnosHastaEmergencia = 0;
    int dia = 1;
    int opcion = 0;
    char error[100];

    srand(time(NULL));
    turnosHastaEmergencia = (rand() % 4) + 4;

    mostrarLogo();
    printf("\n-------------------------- Bienvenido a la sociedad de comunas anarquistas --------------------------\n\n");

    // Crear la sociedad: se leen los archivos y se le piden las cantidades al usuario
    inicio = crearSociedad();
    cantidad_comunas = recorrerComunas(inicio);

    if (inicio == NULL) {
        printf(COLOR_ROJO "No se pudo crear la sociedad.\n" COLOR_RESET);
        return 0;
    }

    if (cantidad_comunas < 3) {
        printf(COLOR_ROJO "Se necesitan al menos 3 comunas para poder administrar\n" COLOR_RESET);
        liberarComunas(inicio);
        return 0;
    }

    elegirComunasAdministradas(inicio, cantidad_comunas);
    comuna_actual = inicio;
    if (comuna_actual->administrado == 0) {
        comuna_actual = siguiente_administrado(comuna_actual);
    }

    actualizarIndices(inicio, cantidad_comunas);

    // Lista de libros de la biblioteca (una ruta por linea en el archivo "libros")
    libros = leerNombresLibros("datos/libros");

    while (i == -1) {
        printf(COLOR_CYAN "---------------------------------------------- Dia %02d -----------------------------------------------\n" COLOR_RESET, dia);
        printf("Comuna actual: %s\n", comuna_actual->nombre);
        printf("1. Hacer un trueque\n");
        printf("2. Pasar al dia siguiente\n");
        printf("3. Ver comunas (necesidad y satisfaccion)\n");
        printf("4. Ver comunas en emergencia\n");
        printf("5. Ver detalle de una comuna (cantidad de cada bien o servicio)\n");
        printf("6. Pasar a la siguiente comuna\n");
        printf("7. Buscar texto en los libros\n");
        printf("8. Salir\n");
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("Elija una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
            scanf("%99s", error);
            continue;
        }

        if (opcion == 1) {
            printf("---------------------------------------------- Trueque ----------------------------------------------\n");
            menuTrueque(inicio, cantidad_comunas, comuna_actual);

        } else if (opcion == 2) {
            printf("------------------------------------- Pasando al dia siguiente --------------------------------------\n");
            printf("Las comunas consumieron bienes.\n");
            pasarTurno(inicio, cantidad_comunas, &turnosHastaEmergencia);
            dia = dia + 1;

        } else if (opcion == 3) {
            printf("---------------------------------------------- Comunas ----------------------------------------------\n");
            mostrarComunas(inicio);

        } else if (opcion == 4) {
            printf(COLOR_ROJO "-------------------------------------------- Emergencias --------------------------------------------\n" COLOR_RESET);
            mostrarEmergencias(inicio);

        } else if (opcion == 5) {
            printf("------------------------------------------ Detalle de comuna ----------------------------------------\n");
            menuDetalleComuna(inicio, cantidad_comunas);

        } else if (opcion == 6) {
            comuna_actual = siguiente_administrado(comuna_actual);
            printf("Se cambio de comuna: %s\n", comuna_actual->nombre);

        } else if (opcion == 7) {
            printf("------------------------------------------ Buscar texto -------------------------------------------\n");
            printf("Texto que desea buscar: ");
            scanf(" %99[^\n]", texto);
            cache = buscarTextoConCache(cache, libros, texto, &resultado);
            mostrarNombresLibro(resultado);

        } else if (opcion == 8) {
            printf(COLOR_MAGENTA "Saliendo.\n" COLOR_RESET);
            i = 0;
        
        } else {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        }
    }

    // Liberar toda la memoria de la sociedad antes de terminar
    liberarComunas(inicio);
    liberarChaflaneado(cache);
    liberarNombresLibro(libros);

    return 0;
}
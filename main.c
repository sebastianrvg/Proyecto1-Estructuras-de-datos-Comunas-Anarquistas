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

void menuTrueque(struct Comuna *inicio, int cantidadComunas) {
    /*
    Funcionamiento: guia al usuario para hacer un trueque: elige la comuna que solicita, la que provee, si es de servicios o bienes, que pide y cuanto. Luego llama a trueque y actualiza los indices.
    Entradas: inicio (puntero a cualquier comuna de la lista circular),
              cantidadComunas (numero total de comunas)
    Salidas: ninguna (si algun dato no es valido, cancela el trueque)
    */
    struct Comuna *actual = inicio;
    struct Comuna *solicitante = NULL;
    struct Comuna *proveedora = NULL;
    struct Servicios *servicio = NULL;
    struct Recursos *recurso = NULL;
    char nombre_item[50]; // Nombre del servicio o bien que se pide
    char error[100];
    int numero_solicitante = 0;
    int numero_proveedora = 0;
    int tipo = 0; // 1 = servicios, 2 = bienes
    int opcion = 0; // Numero del servicio o bien elegido
    int cantidad = 0; // Cantidad que se pide

    if (cantidadComunas < 2) {
        printf(COLOR_AMARILLO "Hacen falta al menos 2 comunas para hacer un trueque.\n" COLOR_RESET);
        return;
    }

    for (int i = 1; i <= cantidadComunas; i++) {
        printf("%d. %s\n", i, actual->nombre);
        actual = actual->siguiente;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    printf("Numero de la comuna que solicita: ");
    if (scanf("%d", &numero_solicitante) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("Numero de la comuna que provee: ");
    if (scanf("%d", &numero_proveedora) != 1) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    if (numero_solicitante < 1 || numero_solicitante > cantidadComunas || numero_proveedora < 1 || numero_proveedora > cantidadComunas) {
        printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        return;
    }
    solicitante = avanzarComunas(inicio, numero_solicitante - 1, cantidadComunas);
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

    actualizarIndices(inicio, cantidad_comunas);

    while (i == -1) {
        printf(COLOR_CYAN "---------------------------------------------- Dia %02d -----------------------------------------------\n" COLOR_RESET, dia);
        printf("1. Hacer un trueque\n");
        printf("2. Pasar al dia siguiente\n");
        printf("3. Ver comunas (necesidad y satisfaccion)\n");
        printf("4. Ver comunas en emergencia\n");
        printf("5. Ver detalle de una comuna (cantidad de cada bien o servicio)\n");
        printf("6. Salir\n");
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("Elija una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
            scanf("%99s", error);
            continue;
        }

        if (opcion == 1) {
            printf("---------------------------------------------- Trueque ----------------------------------------------\n");
            menuTrueque(inicio, cantidad_comunas);

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
            printf(COLOR_MAGENTA "Saliendo.\n" COLOR_RESET);
            i = 0;

        } else {
            printf(COLOR_AMARILLO "Opcion invalida.\n" COLOR_RESET);
        }
    }

    // Liberar toda la memoria de la sociedad antes de terminar
    liberarComunas(inicio);

    return 0;
}
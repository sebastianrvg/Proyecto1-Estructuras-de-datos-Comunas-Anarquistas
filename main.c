#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Nodos.h"
#include "Funciones_Gustavo.h"

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
        printf("Hacen falta al menos 2 comunas para hacer un trueque.\n");
        return;
    }

    for (int i = 1; i <= cantidadComunas; i++) {
        printf("%d. %s\n", i, actual->nombre);
        actual = actual->siguiente;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    printf("Numero de la comuna que solicita: ");
    if (scanf("%d", &numero_solicitante) != 1) {
        printf("Opcion invalida.\n");
        scanf("%99s", error);
        return;
    }
    printf("Numero de la comuna que provee: ");
    if (scanf("%d", &numero_proveedora) != 1) {
        printf("Opcion invalida.\n");
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");
    if (numero_solicitante < 1 || numero_solicitante > cantidadComunas || numero_proveedora < 1 || numero_proveedora > cantidadComunas) {
        printf("Opcion invalida.\n");
        return;
    }
    solicitante = avanzarComunas(inicio, numero_solicitante - 1, cantidadComunas);
    proveedora = avanzarComunas(inicio, numero_proveedora - 1, cantidadComunas);

    printf("Tipo de trueque (1 = servicios, 2 = bienes): ");
    if (scanf("%d", &tipo) != 1) {
        printf("Opcion invalida.\n");
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
        printf("Opcion invalida.\n");
        return;
    }

    if (scanf("%d", &opcion) != 1) {
        printf("Opcion invalida.\n");
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
            printf("Opcion invalida.\n");
            return;
        }
        strcpy(nombre_item, servicio->nombre);
    } else {
        recurso = proveedora->bienes;
        for (int i = 1; i < opcion && recurso != NULL; i++) {
            recurso = recurso->siguiente;
        }
        if (opcion < 1 || recurso == NULL) {
            printf("Opcion invalida.\n");
            return;
        }
        strcpy(nombre_item, recurso->nombre);
    }

    printf("Cantidad que solicita: ");
    if (scanf("%d", &cantidad) != 1) {
        printf("Opcion invalida.\n");
        scanf("%99s", error);
        return;
    }
    printf("-----------------------------------------------------------------------------------------------------\n");

    trueque(solicitante, proveedora, tipo, nombre_item, cantidad);
    actualizarIndices(inicio, cantidadComunas);
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

    printf("\n-------------------------- Bienvenido a la sociedad de comunas anarquistas --------------------------\n\n");

    // PENDIENTE: aqui se crea la sociedad (inicio y cantidad_comunas) pidiendole al usuario las cantidades
    if (inicio == NULL) {
        printf("La sociedad todavia no esta creada.\n");
        return 0;
    }

    actualizarIndices(inicio, cantidad_comunas);

    while (i == -1) {
        printf("---------------------------------------------- Dia %02d -----------------------------------------------\n", dia);
        printf("1. Hacer un trueque\n");
        printf("2. Pasar al dia siguiente\n");
        printf("3. Ver comunas (necesidad y satisfaccion)\n");
        printf("4. Ver comunas en emergencia\n");
        printf("5. Salir\n");
        printf("-----------------------------------------------------------------------------------------------------\n");
        printf("Elija una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Opcion invalida.\n");
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
            printf("-------------------------------------------- Emergencias --------------------------------------------\n");
            mostrarEmergencias(inicio);

        } else if (opcion == 5) {
            printf("Saliendo.\n");
            i = 0;

        } else {
            printf("Opcion invalida.\n");
        }
    }

    return 0;
}
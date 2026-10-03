#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Nodos.h"
#include "Funciones_Gustavo.h"

int main() {
    int turnosHastaEmergencia = 0;

    srand(time(NULL));
    turnosHastaEmergencia = (rand() % 4) + 4;

    return 0;
}
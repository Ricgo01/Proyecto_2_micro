/*
------------------------------------------------------------
Proyecto 2
Optimizacion de tiempo de Cocion
-------------------------------------------------------------
UNIVERSIDAD DEL VALLE DE GUATEMALA
Programación de Microprocesadores
*
Proyecto 2 Fase 2
Descripción: Programa paralelo para optimizar
el tiempo de cocion
El resultado final es la lista de orden de cocion
de los alimentos ingresados

Autores:    Vianka Castro
            Ricardo Arturo
            Paula De Leon
Fecha:       Agosto 20 2024.
-------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ALIMENTOS 100

typedef struct {
    char nombre[50];
    char tiempoPreparacion[20];
    int temperaturaCoccion;
    char tiempoCoccion[20];
    char tiempoEmpaquetado[20];
    int porciones;
} Alimento;

int main() {
    FILE *file = fopen("alimentos_preparacion.csv", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    Alimento alimentos[MAX_ALIMENTOS];
    int count = 0;

    // Leer la primera línea (encabezados)
    fgets(line, sizeof(line), file);

    // Leer los datos y almacenar en la estructura
    while (fgets(line, sizeof(line), file)) {
        char *token;

        // Nombre del Alimento
        token = strtok(line, ",");
        strcpy(alimentos[count].nombre, token);

        // Tiempo de Preparacion
        token = strtok(NULL, ",");
        strcpy(alimentos[count].tiempoPreparacion, token);

        // Temperatura de Coccion (°C)
        token = strtok(NULL, ",");
        alimentos[count].temperaturaCoccion = atoi(token);

        // Tiempo de Coccion
        token = strtok(NULL, ",");
        strcpy(alimentos[count].tiempoCoccion, token);

        // Tiempo de Empaquetado
        token = strtok(NULL, ",");
        strcpy(alimentos[count].tiempoEmpaquetado, token);

        // Porciones por 100kg
        token = strtok(NULL, ",");
        alimentos[count].porciones = atoi(token);

        count++;
    }

    fclose(file);

    // Mostrar los nombres de los alimentos
    printf("Lista de Alimentos:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, alimentos[i].nombre);
    }

    // Pedir al usuario que seleccione un alimento
    int seleccion;
    printf("Seleccione un alimento por su número: ");
    scanf("%d", &seleccion);

    if (seleccion < 1 || seleccion > count) {
        printf("Selección inválida.\n");
        return EXIT_FAILURE;
    }

    // Mostrar las características del alimento seleccionado
    Alimento seleccionado = alimentos[seleccion - 1];
    printf("\nCaracterísticas del alimento seleccionado:\n");
    printf("Nombre: %s\n", seleccionado.nombre);
    printf("Tiempo de Preparación: %s\n", seleccionado.tiempoPreparacion);
    printf("Temperatura de Cocción: %d°C\n", seleccionado.temperaturaCoccion);
    printf("Tiempo de Cocción: %s\n", seleccionado.tiempoCoccion);
    printf("Tiempo de Empaquetado: %s\n", seleccionado.tiempoEmpaquetado);
    printf("Porciones por 100kg: %d\n", seleccionado.porciones);

    return EXIT_SUCCESS;
}
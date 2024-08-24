/*
* ------------------------------------------------------------
* Proyecto 2
* Optimización de Tiempo de Cocción
* -------------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* Programación de Microprocesadores
*
* Proyecto 2 Fase 2
* Descripción: Programa paralelo para optimizar
*              el tiempo de cocción.
*              El resultado final es la lista de orden de cocción
*              de los alimentos ingresados.
*
* Autores:    Vianka Castro
*             Ricardo Arturo
*             Paula De Leon
* Fecha:       Agosto 20 2024.
* -------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <unistd.h> // Para la función sleep()

#define MAX_LINE_LENGTH 1024
#define MAX_ALIMENTOS 100

typedef struct {
    char nombre[50];
    int tiempoPreparacion;
    int temperaturaCoccion;
    int tiempoCoccion;
    int tiempoEmpaquetado;
    int porciones;
} Alimento;

void cargarDatos(Alimento alimentos[], int *count) {
    FILE *file = fopen("alimentos_preparacion.csv", "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    // Leer la primera linea (encabezados)
    fgets(line, sizeof(line), file);

    // Leer los datos y almacenar en la estructura
    while (fgets(line, sizeof(line), file)) {
        char *token;

        // Nombre del Alimento
        token = strtok(line, ",");
        strcpy(alimentos[*count].nombre, token);

        // Tiempo de Preparacion
        token = strtok(NULL, ",");
        alimentos[*count].tiempoPreparacion = atoi(token);

        // Temperatura de Coccion (°C)
        token = strtok(NULL, ",");
        alimentos[*count].temperaturaCoccion = atoi(token);

        // Tiempo de Coccion
        token = strtok(NULL, ",");
        alimentos[*count].tiempoCoccion = atoi(token);

        // Tiempo de Empaquetado
        token = strtok(NULL, ",");
        alimentos[*count].tiempoEmpaquetado = atoi(token);

        // Porciones por 100kg
        token = strtok(NULL, ",");
        alimentos[*count].porciones = atoi(token);

        (*count)++;
    }

    fclose(file);
}

void calcularTiemposInicio(Alimento alimentos[], int seleccion[], int numSeleccionados, int startTime[]) {
    int maxCookTime = 0;

    // Encontrar el tiempo maximo de coccion entre los seleccionados
    for (int i = 0; i < numSeleccionados; i++) {
        int idx = seleccion[i];
        if (alimentos[idx].tiempoCoccion > maxCookTime) {
            maxCookTime = alimentos[idx].tiempoCoccion;
        }
    }

    // Calcular tiempos de inicio para que todos terminen al mismo tiempo
    for (int i = 0; i < numSeleccionados; i++) {
        int idx = seleccion[i];
        startTime[idx] = maxCookTime - alimentos[idx].tiempoCoccion;
    }
}

void cocinarAlimento(const char *nombre, int tiempo) {
    printf("Cocinando %s por %d minutos...\n", nombre, tiempo);
    // Simulacion de tiempo de coccion
    sleep(tiempo);
    printf("%s ha terminado de cocinarse.\n", nombre);
}

int main() {
    Alimento alimentos[MAX_ALIMENTOS];
    int count;
    int startTime[MAX_ALIMENTOS];
    int seleccion[MAX_ALIMENTOS];
    int numSeleccionados = 0;

    cargarDatos(alimentos, &count);

    // Mostrar los nombres de los alimentos
    printf("Lista de Alimentos:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, alimentos[i].nombre);
    }

    // Pedir al usuario que seleccione alimentos
    printf("Seleccione los alimentos por su numero (ingrese 0 para finalizar): \n");
    while (1) {
        int sel;
        scanf("%d", &sel);
        if (sel == 0) break;
        if (sel < 1 || sel > count) {
            printf("Seleccion invalida.\n");
        } else {
            seleccion[numSeleccionados++] = sel - 1;
        }
    }

    // Calcular los tiempos de inicio para que todos los alimentos terminen al mismo tiempo
    calcularTiemposInicio(alimentos, seleccion, numSeleccionados, startTime);

    // Cocinar alimentos en paralelo
    #pragma omp parallel for
    for (int i = 0; i < numSeleccionados; i++) {
        int idx = seleccion[i];
        sleep(startTime[idx]); // Esperar hasta el momento adecuado para empezar a cocinar
        cocinarAlimento(alimentos[idx].nombre, alimentos[idx].tiempoCoccion);
    }

    // Monitorear el progreso
    printf("Todos los alimentos han terminado de cocinarse.\n");

    return EXIT_SUCCESS;
}

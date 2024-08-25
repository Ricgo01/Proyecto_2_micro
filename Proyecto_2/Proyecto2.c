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
#include <windows.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ALIMENTOS 100

// Estructura para almacenar la información de un alimento
typedef struct {
    char nombre[50];
    int tiempoPreparacion;      // Minutos por cada 100kg
    int temperaturaCoccion;     // Grados Celsius
    int tiempoCoccion;          // Minutos por cada 100kg
    int tiempoEmpaquetado;      // Minutos por cada 100kg
    int porciones;              // Porciones por cada 100kg
} Alimento;

// Prototipos de funciones
int menu(Alimento alimentos[], int count);
void agregarAlimento(Alimento alimentos[], int *count);
void modificarAlimento(Alimento alimentos[], int count);
void mostrarAlimentos(Alimento alimentos[], int count);
void comenzarCoccion(Alimento alimentos[], int count);

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

    // Leer los datos y almacenarlos en la estructura
    while (fgets(line, sizeof(line), file)) {
        char *token;

        // Nombre del Alimento
        token = strtok(line, ",");
        strcpy(alimentos[count].nombre, token);

        // Tiempo de Preparación
        token = strtok(NULL, ",");
        alimentos[count].tiempoPreparacion = atoi(token);

        // Temperatura de Cocción (°C)
        token = strtok(NULL, ",");
        alimentos[count].temperaturaCoccion = atoi(token);

        // Tiempo de Cocción
        token = strtok(NULL, ",");
        alimentos[count].tiempoCoccion = atoi(token);

        // Tiempo de Empaquetado
        token = strtok(NULL, ",");
        alimentos[count].tiempoEmpaquetado = atoi(token);

        // Porciones por 100kg
        token = strtok(NULL, ",");
        alimentos[count].porciones = atoi(token);

        count++;
    }

    fclose(file);

    // Mostrar el menú principal
    menu(alimentos, count);

    return 0;
}

// Función para mostrar el menú y gestionar las opciones
int menu(Alimento alimentos[], int count) {
    int eleccion;

    do {
        printf("\n*** Menu de la Planta Alimenticia PVR ***\n");
        printf("1. Ingresar nuevos elementos a la lista de alimentos disponibles\n");
        printf("2. Modificar los alimentos en la lista\n");
        printf("3. Mostrar la lista de alimentos\n");
        printf("4. Comenzar Cocción\n");
        printf("5. Salir del programa\n");
        printf("Seleccione una opción: ");
        scanf("%d", &eleccion);

        switch (eleccion) {
            case 1: 
                agregarAlimento(alimentos, &count);
                break;
            case 2:
                modificarAlimento(alimentos, count);
                break;
            case 3:
                mostrarAlimentos(alimentos, count);
                break;
            case 4:
                comenzarCoccion(alimentos, count);
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Por favor intente de nuevo.\n");
                break;
        }

    } while (eleccion != 5);

    return 0;
}

// Función para agregar un nuevo alimento
void agregarAlimento(Alimento alimentos[], int *count) {
    Alimento nAlimento;

    printf("\n--- Agregar Nuevo Alimento ---\n");
    printf("Ingrese el nombre del alimento: ");
    scanf("%s", nAlimento.nombre);

    printf("Ingrese el tiempo en minutos por cada 100kg de alimento en preparación previa: ");
    scanf("%d", &nAlimento.tiempoPreparacion);

    printf("Ingrese la temperatura de cocción en grados Celsius: ");
    scanf("%d", &nAlimento.temperaturaCoccion);

    printf("Ingrese el tiempo de cocción en minutos por cada 100kg: ");
    scanf("%d", &nAlimento.tiempoCoccion);

    printf("Ingrese el tiempo de empaquetado en minutos por cada 100kg: ");
    scanf("%d", &nAlimento.tiempoEmpaquetado);

    printf("Ingrese las porciones por cada 100kg de alimento: ");
    scanf("%d", &nAlimento.porciones);

    // Guardar el nuevo alimento en el archivo CSV
    FILE *file = fopen("alimentos_preparacion.csv", "a");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fprintf(file, "%s,%d,%d,%d,%d,%d\n",
            nAlimento.nombre,
            nAlimento.tiempoPreparacion,
            nAlimento.temperaturaCoccion,
            nAlimento.tiempoCoccion,
            nAlimento.tiempoEmpaquetado,
            nAlimento.porciones);

    fclose(file);

    // Añadir el nuevo alimento al arreglo de alimentos
    alimentos[*count] = nAlimento;
    (*count)++;

    printf("Alimento agregado exitosamente.\n");
}

// Función para modificar un alimento existente
void modificarAlimento(Alimento alimentos[], int count) {
    int seleccion;
    printf("\n--- Modificar Alimento ---\n");
    mostrarAlimentos(alimentos, count);

    printf("Seleccione el número del alimento que desea modificar: ");
    scanf("%d", &seleccion);

    if (seleccion < 1 || seleccion > count) {
        printf("Selección inválida.\n");
        return;
    }

    Alimento *alimento = &alimentos[seleccion - 1];

    printf("Modificar (deje en blanco para mantener el valor actual):\n");

    printf("Nombre actual: %s\n", alimento->nombre);
    printf("Ingrese el nuevo nombre del alimento: ");
    scanf("%s", alimento->nombre);

    printf("Tiempo de Preparación actual (min/100kg): %d\n", alimento->tiempoPreparacion);
    printf("Ingrese el nuevo tiempo de preparación: ");
    scanf("%d", &alimento->tiempoPreparacion);

    printf("Temperatura de Cocción actual (°C): %d\n", alimento->temperaturaCoccion);
    printf("Ingrese la nueva temperatura de cocción: ");
    scanf("%d", &alimento->temperaturaCoccion);

    printf("Tiempo de Cocción actual (min/100kg): %d\n", alimento->tiempoCoccion);
    printf("Ingrese el nuevo tiempo de cocción: ");
    scanf("%d", &alimento->tiempoCoccion);

    printf("Tiempo de Empaquetado actual (min/100kg): %d\n", alimento->tiempoEmpaquetado);
    printf("Ingrese el nuevo tiempo de empaquetado: ");
    scanf("%d", &alimento->tiempoEmpaquetado);

    printf("Porciones actuales por 100kg: %d\n", alimento->porciones);
    printf("Ingrese las nuevas porciones por cada 100kg de alimento: ");
    scanf("%d", &alimento->porciones);

    printf("Alimento modificado exitosamente.\n");
}

// Función para mostrar la lista de alimentos
void mostrarAlimentos(Alimento alimentos[], int count) {
    printf("\n--- Lista de Alimentos ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, alimentos[i].nombre);
        //printf("   Tiempo de Preparación: %d min/100kg\n", alimentos[i].tiempoPreparacion);
        //printf("   Temperatura de Cocción: %d°C\n", alimentos[i].temperaturaCoccion);
        //printf("   Tiempo de Cocción: %d min/100kg\n", alimentos[i].tiempoCoccion);
        //printf("   Tiempo de Empaquetado: %d min/100kg\n", alimentos[i].tiempoEmpaquetado);
        //printf("   Porciones por 100kg: %d\n", alimentos[i].porciones);
    }
}


// Función para la paralelización con OpenMP
void comenzarCoccion(Alimento alimentos[], int count) {
      int seleccion[MAX_ALIMENTOS];
    int numAlimentos;

    printf("Seleccione los alimentos que desea cocinar (ingrese el número del alimento, 0 para terminar):\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, alimentos[i].nombre);
    }

    numAlimentos = 0;
    while (1) {
        int seleccionTemp;
        printf("Seleccione el número del alimento (0 para terminar): ");
        scanf("%d", &seleccionTemp);
        if (seleccionTemp == 0) break;
        if (seleccionTemp < 1 || seleccionTemp > count) {
            printf("Selección inválida. Intente nuevamente.\n");
        } else {
            seleccion[numAlimentos++] = seleccionTemp - 1;
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < numAlimentos; i++) {
        int idx = seleccion[i];
        Alimento a = alimentos[idx];

        printf("Preparando %s...\n", a.nombre);
        // Simular preparación
        Sleep(a.tiempoPreparacion * 1000); // Convertir a milisegundos

        printf("Cocinando %s...\n", a.nombre);
        // Calcular tiempo de cocción en función de la temperatura
        //FALTA IMPLEMENTARLO
        //Sleep((int)tiempoCoccion * 1000); // Convertir a milisegundos

        printf("Empaquetando %s...\n", a.nombre);
        // Simular empaquetado
        Sleep(a.tiempoEmpaquetado * 1000); // Convertir a milisegundos

        printf("%s terminado.\n", a.nombre);
    }
   
}

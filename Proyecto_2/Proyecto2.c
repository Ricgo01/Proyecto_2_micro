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
#include <omp.h>

#define MAX_ALIMENTOS 100
#define MAX_LINE_LENGTH 1024
#define THREADS 4

typedef struct {
    char nombre[50];
    int tiempoPreparacion;      
    int temperaturaCoccion;     
    int tiempoCoccion;          
    int tiempoEmpaquetado;      
    int porciones;              
    int kg;                     
    int temperaturaIngresada;   
} Alimento;

void prepararAlimento(Alimento *a) {
    // Validar kg antes de usarlo
    if (a->kg < 0 || a->kg > 100000) {
        printf("Error: Valor de kg inválido para %s (%d kg)\n", a->nombre, a->kg);
        return;
    }
    int tiempoTotal = (a->tiempoPreparacion * a->kg) / 100;
    printf("Hilo %d: Preparando %s para %d kg...\n", omp_get_thread_num(), a->nombre, a->kg);
    Sleep(tiempoTotal * 1000);
}

void cocinarAlimento(Alimento *a) {
    // Validar temperatura antes de usarla
    if (a->temperaturaIngresada < 0 || a->temperaturaIngresada > 500) {
        printf("Error: Valor de temperatura inválido para %s (%d grados Celsius)\n", a->nombre, a->temperaturaIngresada);
        return;
    }

    int tiempoTotal = (a->tiempoCoccion * a->kg) / 100;

    if (a->temperaturaIngresada != a->temperaturaCoccion) {
        double ajuste = (double)a->temperaturaCoccion / a->temperaturaIngresada;
        tiempoTotal *= ajuste;
    }

    printf("Hilo %d: Cocinando %s a %d grados Celsius...\n", omp_get_thread_num(), a->nombre, a->temperaturaIngresada);
    Sleep(tiempoTotal * 1000);
}

void empaquetarAlimento(Alimento *a) {
    int tiempoTotal = (a->tiempoEmpaquetado * a->kg) / 100;
    printf("Hilo %d: Empaquetando %s...\n", omp_get_thread_num(), a->nombre);
    Sleep(tiempoTotal * 1000);
}

void procesarAlimento(Alimento *a) {
    prepararAlimento(a);
    cocinarAlimento(a);
    empaquetarAlimento(a);
    printf("Hilo %d: %s terminado. Se obtuvieron %d porciones.\n", omp_get_thread_num(), a->nombre, (a->porciones * a->kg) / 100);
}

int cargarAlimentosDesdeCSV(const char *nombreArchivo, Alimento alimentos[]) {
    FILE *file = fopen(nombreArchivo, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char *token;

        token = strtok(line, ",");
        strcpy(alimentos[count].nombre, token);

        token = strtok(NULL, ",");
        alimentos[count].tiempoPreparacion = atoi(token);

        token = strtok(NULL, ",");
        alimentos[count].temperaturaCoccion = atoi(token);

        token = strtok(NULL, ",");
        alimentos[count].tiempoCoccion = atoi(token);

        token = strtok(NULL, ",");
        alimentos[count].tiempoEmpaquetado = atoi(token);

        token = strtok(NULL, ",");
        alimentos[count].porciones = atoi(token);

        alimentos[count].kg = 0; // Inicializamos kg para evitar valores basura
        alimentos[count].temperaturaIngresada = 0; // Inicializamos la temperatura para evitar valores basura

        count++;
    }

    fclose(file);
    return count;
}

void guardarAlimentosEnCSV(const char *nombreArchivo, Alimento alimentos[], int count) {
    FILE *file = fopen(nombreArchivo, "w");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fprintf(file, "Nombre,TiempoPreparacion,TemperaturaCoccion,TiempoCoccion,TiempoEmpaquetado,Porciones\n");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d,%d,%d,%d,%d\n",
                alimentos[i].nombre,
                alimentos[i].tiempoPreparacion,
                alimentos[i].temperaturaCoccion,
                alimentos[i].tiempoCoccion,
                alimentos[i].tiempoEmpaquetado,
                alimentos[i].porciones);
    }

    fclose(file);
}

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

    nAlimento.kg = 0; // Inicializa kg
    nAlimento.temperaturaIngresada = 0; // Inicializa temperatura ingresada

    alimentos[*count] = nAlimento;
    (*count)++;

    printf("Alimento agregado exitosamente.\n");
}

void modificarAlimento(Alimento alimentos[], int count) {
    int seleccion;
    printf("\n--- Modificar Alimento ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, alimentos[i].nombre);
    }

    printf("Seleccione el número del alimento que desea modificar: ");
    scanf("%d", &seleccion);

    if (seleccion < 1 || seleccion > count) {
        printf("Selección inválida.\n");
        return;
    }

    Alimento *alimento = &alimentos[seleccion - 1];

    printf("Modificar los valores para %s (deje en blanco para mantener el valor actual):\n", alimento->nombre);

    printf("Tiempo de Preparación actual (min/100kg): %d\n", alimento->tiempoPreparacion);
    printf("Ingrese el nuevo tiempo de preparación: ");
    scanf("%d", &alimento->tiempoPreparacion);

    printf("Temperatura de Cocción actual (grados Celsius): %d\n", alimento->temperaturaCoccion);
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

void mostrarAlimentos(Alimento alimentos[], int count) {
    printf("\n--- Lista de Alimentos ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, alimentos[i].nombre);
    }
}

void comenzarProcesoDeCoccion(Alimento alimentos[], int count) {
    int seleccion[MAX_ALIMENTOS];
    int numSeleccionados = 0;

    printf("\n--- Selección de Alimentos para Cocinar ---\n");
    mostrarAlimentos(alimentos, count);

    printf("Seleccione los alimentos que desea cocinar (ingrese el número del alimento, 0 para terminar):\n");
    while (1) {
        int opcion;
        printf("Seleccione el número del alimento: ");
        scanf("%d", &opcion);
        if (opcion == 0) {
            break;
        } else if (opcion < 1 || opcion > count) {
            printf("Selección inválida. Intente de nuevo.\n");
        } else {
            seleccion[numSeleccionados++] = opcion - 1;
        }
    }

    if (numSeleccionados == 0) {
        printf("No se seleccionaron alimentos para cocinar.\n");
        return;
    }

    omp_set_num_threads(THREADS);

    for (int i = 0; i < numSeleccionados; i++) {
        int idx = seleccion[i];
        do {
            printf("Ingrese la cantidad de kilogramos que desea procesar para %s (múltiplo de 100): ", alimentos[idx].nombre);
            scanf("%d", &alimentos[idx].kg);
            if (alimentos[idx].kg % 100 != 0) {
                printf("Error: La cantidad debe ser un múltiplo de 100. Intente de nuevo.\n");
            }
        } while (alimentos[idx].kg % 100 != 0);

        printf("La temperatura recomendada para %s es de %d grados Celsius.\n", alimentos[idx].nombre, alimentos[idx].temperaturaCoccion);
        printf("Ingrese la temperatura a la que desea cocinar %s: ", alimentos[idx].nombre);
        scanf("%d", &alimentos[idx].temperaturaIngresada);

        // Validar los valores
        if (alimentos[idx].kg < 0 || alimentos[idx].kg > 100000 || alimentos[idx].temperaturaIngresada < 0 || alimentos[idx].temperaturaIngresada > 500) {
            printf("Error: Valores inválidos para %s (kg: %d, temperatura: %d)\n", alimentos[idx].nombre, alimentos[idx].kg, alimentos[idx].temperaturaIngresada);
            return;
        }
    }

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < numSeleccionados; i++) {
        procesarAlimento(&alimentos[seleccion[i]]);
    }
}

int menu(Alimento alimentos[], int *count) {
    int eleccion;

    do {
        printf("\n*** Menu de la Planta Alimenticia ***\n");
        printf("1. Ingresar nuevo alimento\n");
        printf("2. Modificar un alimento\n");
        printf("3. Mostrar lista de alimentos\n");
        printf("4. Seleccionar y cocinar alimentos\n");
        printf("5. Guardar y salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &eleccion);

        switch (eleccion) {
            case 1:
                agregarAlimento(alimentos, count);
                break;
            case 2:
                modificarAlimento(alimentos, *count);
                break;
            case 3:
                mostrarAlimentos(alimentos, *count);
                break;
            case 4:
                comenzarProcesoDeCoccion(alimentos, *count);
                break;
            case 5:
                guardarAlimentosEnCSV("alimentos_preparacion.csv", alimentos, *count);
                printf("Datos guardados. Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Por favor intente de nuevo.\n");
                break;
        }

    } while (eleccion != 5);

    return 0;
}

int main() {
    Alimento alimentos[MAX_ALIMENTOS];
    int count = cargarAlimentosDesdeCSV("alimentos_preparacion.csv", alimentos);

    if (count >= 0) {
        menu(alimentos, &count);
    } else {
        printf("Error al cargar los alimentos.\n");
    }

    return 0;
}

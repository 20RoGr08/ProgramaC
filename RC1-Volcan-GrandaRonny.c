#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// CONSTANTES DEL SISTEMA
#define MAX_VOLCANES     50    
#define MAX_NOMBRE       60    

//Umbrales SAVE / SNGRE (2022)
//Sismicidad
#define SISMO_AMARILLO   10
#define SISMO_NARANJA    30
#define SISMO_ROJO       60

//SO2 (ppm)
#define SO2_AMARILLO     500
#define SO2_NARANJA      2000
#define SO2_ROJO         5000

//Temperatura del crater (C)
#define TEMP_AMARILLO    200
#define TEMP_NARANJA     400
#define TEMP_ROJO        700

//ESTRUCTURA DE DATOS
typedef struct {
    char   nombre[MAX_NOMBRE];   //Nombre del volcan                  
    int    sismicidad[24];       //Eventos sismicos por hora (24 h)   
    float  so2;                  //Concentracion SO2 (ppm)            
    float  temperatura;          //Temperatura superficial crater (C)
} Volcan;

//VARIABLES GLOBALES

Volcan volcanes[MAX_VOLCANES];
int    totalVolcanes = 0;


//PROTOTIPOS DE FUNCIONES

void   mostrarMenu(void);
void   ingresarVolcan(void);
void   editarVolcan(void);
void   eliminarVolcan(void);
void   calcularPromedioSismico(void);
void   determinarAlertaGeneral(void);
void   volcanesEnAlertaCritica(void);
void   totalEventosSismicos(void);
void   mostrarTodos(void);
int    buscarPorNombre(const char *nombre);
float  promedioArreglo(const int *arr, int n);
void   determinarAlertaVolcan(const Volcan *v, char *nivel);
void   limpiarBuffer(void);
void   linea(void);

//FUNCION PRINCIPAL
int main(void) {
    int opcion;

    printf("\n");
    linea();
    printf("  SISTEMA DE MONITOREO VOLCANICO\n");
    printf("  Ecuador - SAVE (Sistema de Alerta Volcanica)\n");
    linea();

    do {
        mostrarMenu();
        printf("  Seleccione una opcion: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 1:  ingresarVolcan();          break;
            case 2:  editarVolcan();            break;
            case 3:  eliminarVolcan();          break;
            case 4:  calcularPromedioSismico(); break;
            case 5:  determinarAlertaGeneral(); break;
            case 6:  volcanesEnAlertaCritica(); break;
            case 7:  totalEventosSismicos();    break;
            case 8:  mostrarTodos();            break;
            case 0:
                printf("\n  Saliendo del sistema. Hasta pronto\n\n");
                break;
            default:
                printf("\n  [!] Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 0);

    return 0;
}

//MENU PRINCIPAL
void mostrarMenu(void) {
    printf("\n");
    linea();
    printf("  MENU PRINCIPAL\n");
    linea();
    printf("  1. Ingresar datos de un volcan\n");
    printf("  2. Editar datos de un volcan\n");
    printf("  3. Eliminar un volcan del registro\n");
    printf("  4. Calcular promedio diario de sismicidad\n");
    printf("  5. Determinar nivel de alerta de todos los volcanes\n");
    printf("  6. Listar volcanes en alerta Naranja o Rojo\n");
    printf("  7. Total acumulado de eventos sismicos (24 h)\n");
    printf("  8. Mostrar todos los volcanes registrados\n");
    printf("  0. Salir\n");
    linea();
}

//1. INGRESAR UN VOLCAN

void ingresarVolcan(void) {
    int i;
    printf("\nINGRESO DE VOLCAN\n");

    if (totalVolcanes >= MAX_VOLCANES) {
        printf("  [!] Registro lleno. Elimine un volcan antes de ingresar otro.\n");
        return;
    }

    Volcan *v = &volcanes[totalVolcanes];

    printf("  Nombre del volcan: ");
    fgets(v->nombre, MAX_NOMBRE, stdin);
    //Quitar salto de linea
    v->nombre[strcspn(v->nombre, "\n")] = '\0';

    //Verificar duplicado
    if (buscarPorNombre(v->nombre) != -1) {
        printf("  [!] Ya existe un volcan con ese nombre.\n");
        return;
    }

    printf("  Concentracion de SO2 (ppm): ");
    scanf("%f", &v->so2);

    printf("  Temperatura del crater (C): ");
    scanf("%f", &v->temperatura);
    limpiarBuffer();

    printf("  Ingrese los eventos sismicos por hora para las 24 horas del dia:\n");
    for (i = 0; i < 24; i++) {
        printf("    Hora %02d:00 -> ", i);
        scanf("%d", &v->sismicidad[i]);
    }
    limpiarBuffer();

    totalVolcanes++;
    printf("\n  [OK] Volcan '%s' registrado exitosamente.\n", v->nombre);
}

//2. EDITAR UN VOLCAN (busqueda por nombre)

void editarVolcan(void) {
    char nombre[MAX_NOMBRE];
    int  idx, i;

    printf("\nEDICION DE VOLCAN\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    printf("  Nombre del volcan a editar: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    idx = buscarPorNombre(nombre);
    if (idx == -1) { printf("  [!] Volcan no encontrado.\n"); return; }

    Volcan *v = &volcanes[idx];
    printf("  Volcan encontrado: %s\n", v->nombre);
    printf("  Ingrese los nuevos datos:\n");

    printf("  Concentracion de SO2 (ppm): ");
    scanf("%f", &v->so2);

    printf("  Temperatura del crater (C): ");
    scanf("%f", &v->temperatura);
    limpiarBuffer();

    printf("  Ingrese los eventos sismicos por hora para las 24 horas:\n");
    for (i = 0; i < 24; i++) {
        printf("    Hora %02d:00 -> ", i);
        scanf("%d", &v->sismicidad[i]);
    }
    limpiarBuffer();

    printf("\n  [OK] Datos del volcan '%s' actualizados.\n", v->nombre);
}

//3. ELIMINAR UN VOLCAN (busqueda por nombre)
void eliminarVolcan(void) {
    char nombre[MAX_NOMBRE];
    int  idx, i;

    printf("\nELIMINACION DE VOLCAN\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    printf("  Nombre del volcan a eliminar: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    idx = buscarPorNombre(nombre);
    if (idx == -1) { printf("  [!] Volcan no encontrado.\n"); return; }

    /* Desplazar elementos (sin funciones de ordenamiento predefinidas) */
    for (i = idx; i < totalVolcanes - 1; i++) {
        volcanes[i] = volcanes[i + 1];
    }
    totalVolcanes--;
    printf("\n  [OK] Volcan '%s' eliminado del registro.\n", nombre);
}

//4. CALCULAR PROMEDIO DIARIO DE SISMICIDAD
void calcularPromedioSismico(void) {
    int i;
    printf("\nPROMEDIO DIARIO DE SISMICIDAD\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    for (i = 0; i < totalVolcanes; i++) {
        float prom = promedioArreglo(volcanes[i].sismicidad, 24);
        printf("  %-30s -> Promedio: %.2f eventos/hora\n", volcanes[i].nombre, prom);
    }
}

//5. DETERMINAR NIVEL DE ALERTA: todos

void determinarAlertaGeneral(void) {
    int  i;
    char nivel[10];

    printf("\nNIVEL DE ALERTA VOLCANICA (SAVE)\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    printf("  %-30s %-12s %-12s %-12s %-10s\n",
           "VOLCAN", "SISMO prom.", "SO2 (ppm)", "TEMP (C)", "ALERTA");
    linea();

    for (i = 0; i < totalVolcanes; i++) {
        float prom = promedioArreglo(volcanes[i].sismicidad, 24);
        determinarAlertaVolcan(&volcanes[i], nivel);
        printf("  %-30s %-12.2f %-12.1f %-12.1f %-10s\n",
               volcanes[i].nombre, prom,
               volcanes[i].so2, volcanes[i].temperatura, nivel);
    }
}

//6. VOLCANES EN ALERTA NARANJA O ROJO
void volcanesEnAlertaCritica(void) {
    int  i, conteo = 0;
    char nivel[10];

    printf("\nVOLCANES EN ALERTA NARANJA O ROJO\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    for (i = 0; i < totalVolcanes; i++) {
        determinarAlertaVolcan(&volcanes[i], nivel);
        if (strcmp(nivel, "NARANJA") == 0 || strcmp(nivel, "ROJO") == 0) {
            printf("  [%s] %s\n", nivel, volcanes[i].nombre);
            conteo++;
        }
    }

    if (conteo == 0) {
        printf("  Ningun volcan en alerta critica actualmente.\n");
    } else {
        printf("\n  Total de volcanes en alerta critica: %d\n", conteo);
    }
}

//7. TOTAL ACUMULADO DE EVENTOS SISMICOS (24 h)
void totalEventosSismicos(void) {
    int i, j;
    long long total = 0;

    printf("\nTOTAL ACUMULADO DE EVENTOS SISMICOS (24 h)\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    for (i = 0; i < totalVolcanes; i++) {
        int subtotal = 0;
        for (j = 0; j < 24; j++) subtotal += volcanes[i].sismicidad[j];
        printf("  %-30s -> %d eventos en 24 h\n", volcanes[i].nombre, subtotal);
        total += subtotal;
    }
    printf("\n  TOTAL GENERAL (todos los volcanes): %lld eventos\n", total);
}

//8. MOSTRAR TODOS LOS VOLCANES
void mostrarTodos(void) {
    int  i, j;
    char nivel[10];

    printf("\nLISTADO COMPLETO DE VOLCANES REGISTRADOS\n");
    if (totalVolcanes == 0) { printf("  [!] No hay volcanes registrados.\n"); return; }

    for (i = 0; i < totalVolcanes; i++) {
        determinarAlertaVolcan(&volcanes[i], nivel);
        printf("\n  [%d] %s\n", i + 1, volcanes[i].nombre);
        printf("      SO2          : %.2f ppm\n",    volcanes[i].so2);
        printf("      Temperatura  : %.2f °C\n",     volcanes[i].temperatura);
        printf("      Promedio     : %.2f ev/h\n",   promedioArreglo(volcanes[i].sismicidad, 24));
        printf("      Alerta       : %s\n",          nivel);
        printf("      Sismicidad (por hora):\n      ");
        for (j = 0; j < 24; j++) {
            printf("%02d:%-4d ", j, volcanes[i].sismicidad[j]);
            if ((j + 1) % 6 == 0) printf("\n      ");
        }
        printf("\n");
    }
}

//FUNCION: Busqueda lineal por nombre (sin funciones predefinidas) Retorna el indice o -1 si no existe.
int buscarPorNombre(const char *nombre) {
    int i, j, igual;
    for (i = 0; i < totalVolcanes; i++) {
        /* Comparacion caracter a caracter (sin strcmp en busqueda) */
        igual = 1;
        for (j = 0; volcanes[i].nombre[j] != '\0' || nombre[j] != '\0'; j++) {
            if (volcanes[i].nombre[j] != nombre[j]) { igual = 0; break; }
        }
        if (igual) return i;
    }
    return -1;
}

//FUNCION: Promedio de un arreglo de enteros
float promedioArreglo(const int *arr, int n) {
    int i;
    int suma = 0;
    for (i = 0; i < n; i++) suma += arr[i];
    return (n > 0) ? (float)suma / n : 0.0f;
}

//FUNCION: Determinar nivel de alerta de un volcan Logica basada en umbrales SAVE / SNGRE (2022). Se usa el criterio MAS CRITICO entre los tres parametros. Nivel: VERDE < AMARILLO < NARANJA < ROJO
void determinarAlertaVolcan(const Volcan *v, char *nivel) {
    int nivelNum = 0; /* 0=Verde, 1=Amarillo, 2=Naranja, 3=Rojo */
    float prom = promedioArreglo(v->sismicidad, 24);

    //Evaluar sismicidad
    if      (prom >= SISMO_ROJO)    nivelNum = (nivelNum < 3) ? 3 : nivelNum;
    else if (prom >= SISMO_NARANJA) nivelNum = (nivelNum < 2) ? 2 : nivelNum;
    else if (prom >= SISMO_AMARILLO)nivelNum = (nivelNum < 1) ? 1 : nivelNum;

    //Evaluar SO2
    if      (v->so2 >= SO2_ROJO)    nivelNum = (nivelNum < 3) ? 3 : nivelNum;
    else if (v->so2 >= SO2_NARANJA) nivelNum = (nivelNum < 2) ? 2 : nivelNum;
    else if (v->so2 >= SO2_AMARILLO)nivelNum = (nivelNum < 1) ? 1 : nivelNum;

    //Evaluar temperatura
    if      (v->temperatura >= TEMP_ROJO)    nivelNum = (nivelNum < 3) ? 3 : nivelNum;
    else if (v->temperatura >= TEMP_NARANJA) nivelNum = (nivelNum < 2) ? 2 : nivelNum;
    else if (v->temperatura >= TEMP_AMARILLO)nivelNum = (nivelNum < 1) ? 1 : nivelNum;

    switch (nivelNum) {
        case 3:  strcpy(nivel, "ROJO");    break;
        case 2:  strcpy(nivel, "NARANJA"); break;
        case 1:  strcpy(nivel, "AMARILLO");break;
        default: strcpy(nivel, "VERDE");   break;
    }
}

//UTILIDADES
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void linea(void) {
    printf("  ================================================================\n");
}

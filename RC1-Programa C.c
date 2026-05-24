#include <stdio.h>
 
#define MAX_PRODUCTOS   5
#define MAX_NOMBRE      50
#define MAX_RECURSOS    3  
#define TIEMPO_MAX      500.0
 
// Arreglos paralelos
char   nombres[MAX_PRODUCTOS][MAX_NOMBRE];
int    cantidades[MAX_PRODUCTOS];
double tiempos_unit[MAX_PRODUCTOS];
int    registrado[MAX_PRODUCTOS];
 
// Matrices paralelas indexadas por [Producto][Recurso]
int    cantidad_tipos_recursos[MAX_PRODUCTOS];
char   nombres_recursos[MAX_PRODUCTOS][MAX_RECURSOS][MAX_NOMBRE];
double recursos_unit[MAX_PRODUCTOS][MAX_RECURSOS];
 
// INVENTARIO PREDETERMINADO
#define TOTAL_INSUMOS_FIJOS  8

char   inventario_nombres[TOTAL_INSUMOS_FIJOS][MAX_NOMBRE];
double inventario_max[TOTAL_INSUMOS_FIJOS];
 
// FUNCIONES AUXILIARES
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 
void leer_cadena(char *d, int m) {
    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && c != EOF && i < m - 1) d[i++] = c;
    d[i] = '\0';
}
 
int comparar_cadenas(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return (int)a[i] - (int)b[i];
}
 
void copiar_cadena(char *d, const char *o) {
    int i = 0;
    while (o[i]) { d[i] = o[i]; i++; }
    d[i] = '\0';
}
 
int buscar_por_nombre(char *nb) {
    for (int i = 0; i < MAX_PRODUCTOS; i++)
        if (registrado[i] && comparar_cadenas(nombres[i], nb) == 0) return i;
    return -1;
}
 
int buscar_en_inventario(char *nb) {
    for (int i = 0; i < TOTAL_INSUMOS_FIJOS; i++) {
        if (comparar_cadenas(inventario_nombres[i], nb) == 0) return i;
    }
    return -1;
}

// INICIALIZACION DEL INVENTARIO PREDETERMINADO
void inicializar_inventario() {
    // Nombres de los insumos de la reposteria
    copiar_cadena(inventario_nombres[0], "Harina");
    copiar_cadena(inventario_nombres[1], "Azucar");
    copiar_cadena(inventario_nombres[2], "Huevos");
    copiar_cadena(inventario_nombres[3], "Mantequilla");
    copiar_cadena(inventario_nombres[4], "Leche");
    copiar_cadena(inventario_nombres[5], "Chocolate");
    copiar_cadena(inventario_nombres[6], "Sal");
    copiar_cadena(inventario_nombres[7], "Levadura");

    // Stock inicial en cero - se debe reabastecer antes de verificar viabilidad
    for (int i = 0; i < TOTAL_INSUMOS_FIJOS; i++) {
        inventario_max[i] = 0.0;
    }
}
 
// FUNCIONES DEL MENU
void ingresar_producto() {
    int i, j, num_rec, idx = -1;
    char nt[MAX_NOMBRE];
    
    for (i = 0; i < MAX_PRODUCTOS; i++) if (!registrado[i]) { idx = i; break; }
    if (idx == -1) { printf("\nERROR: Limite de 5 productos alcanzado.\n"); return; }
 
    printf("\nNombre del producto (ej: Torta de Chocolate, Cupcake Vainilla): ");
    leer_cadena(nt, MAX_NOMBRE);
    if (nt[0] == '\0' || buscar_por_nombre(nt) != -1) { printf("ERROR: Nombre vacio o ya existente.\n"); return; }
 
    printf("Cantidad demandada (unidades): ");
    if (scanf("%d", &cantidades[idx]) != 1 || cantidades[idx] <= 0) { printf("Invalido.\n"); limpiar_buffer(); return; }
    
    printf("Tiempo de preparacion unitario (horas): ");
    if (scanf("%lf", &tiempos_unit[idx]) != 1 || tiempos_unit[idx] <= 0) { printf("Invalido.\n"); limpiar_buffer(); return; }
    
    printf("\nInsumos disponibles en la reposteria:\n");
    for (i = 0; i < TOTAL_INSUMOS_FIJOS; i++) {
        printf("  %d. %s\n", i + 1, inventario_nombres[i]);
    }

    printf("\nCuantos insumos diferentes requiere este producto? (Maximo %d): ", MAX_RECURSOS);
    if (scanf("%d", &num_rec) != 1 || num_rec < 1 || num_rec > MAX_RECURSOS) { printf("Cantidad invalida.\n"); limpiar_buffer(); return; }
    limpiar_buffer();
 
    cantidad_tipos_recursos[idx] = num_rec;
 
    for (j = 0; j < num_rec; j++) {
        char insumo_temp[MAX_NOMBRE];
        int idx_inv;

        printf("  -> Nombre del insumo #%d (debe coincidir exactamente con la lista): ", j + 1);
        leer_cadena(insumo_temp, MAX_NOMBRE);

        idx_inv = buscar_en_inventario(insumo_temp);
        if (idx_inv == -1) {
            printf("     AVISO: '%s' no esta en el inventario de la reposteria. Verifique el nombre.\n", insumo_temp);
        }

        copiar_cadena(nombres_recursos[idx][j], insumo_temp);
        
        printf("     Cantidad de '%s' por unidad de producto: ", nombres_recursos[idx][j]);
        scanf("%lf", &recursos_unit[idx][j]);
        limpiar_buffer();
    }
 
    copiar_cadena(nombres[idx], nt);
    registrado[idx] = 1;
    printf("Producto registrado con exito\n");
}
 
void editar_producto() {
    char nb[MAX_NOMBRE];
    int idx, op, j;
    printf("\nNombre del producto a editar: ");
    leer_cadena(nb, MAX_NOMBRE);
    idx = buscar_por_nombre(nb);
    if (idx == -1) { printf("Producto no encontrado.\n"); return; }
 
    printf("Que desea editar?\n1. Cantidad\n2. Tiempo\n3. Re-ingresar Receta\nOpcion: ");
    if (scanf("%d", &op) != 1) { limpiar_buffer(); return; }
    limpiar_buffer();
    
    if (op == 1) {
        printf("Nueva cantidad: "); scanf("%d", &cantidades[idx]); limpiar_buffer();
    } else if (op == 2) {
        printf("Nuevo tiempo: "); scanf("%lf", &tiempos_unit[idx]); limpiar_buffer();
    } else if (op == 3) {
        printf("Cuantos insumos requiere ahora? (Max %d): ", MAX_RECURSOS);
        scanf("%d", &cantidad_tipos_recursos[idx]); limpiar_buffer();
        printf("\nInsumos disponibles:\n");
        for (j = 0; j < TOTAL_INSUMOS_FIJOS; j++) {
            printf("  %d. %s\n", j + 1, inventario_nombres[j]);
        }
        for (j = 0; j < cantidad_tipos_recursos[idx]; j++) {
            printf("  -> Nombre del insumo #%d: ", j + 1);
            leer_cadena(nombres_recursos[idx][j], MAX_NOMBRE);
            printf("     Cantidad por unidad: ");
            scanf("%lf", &recursos_unit[idx][j]); limpiar_buffer();
        }
    }
    printf("Producto editado correctamente\n");
}
 
void eliminar_producto() {
    char nb[MAX_NOMBRE];
    printf("\nNombre del producto a eliminar: ");
    leer_cadena(nb, MAX_NOMBRE);
    int idx = buscar_por_nombre(nb);
    if (idx != -1) { registrado[idx] = 0; printf("Producto eliminado.\n"); }
    else printf("Producto no encontrado.\n");
}

// REABASTECER INVENTARIO
void reabastecer_inventario() {
    int i;
    double cant;

    printf("\nREABASTECIMIENTO DE INVENTARIO - Dulce Arte\n");
    printf("Insumos disponibles para reabastecer:\n");
    for (i = 0; i < TOTAL_INSUMOS_FIJOS; i++) {
        printf("  %d. %-15s | Stock actual: %.2f\n", i + 1, inventario_nombres[i], inventario_max[i]);
    }

    printf("\nSeleccione el numero del insumo a reabastecer (1-%d): ", TOTAL_INSUMOS_FIJOS);
    int sel;
    if (scanf("%d", &sel) != 1 || sel < 1 || sel > TOTAL_INSUMOS_FIJOS) {
        printf("Seleccion invalida.\n"); limpiar_buffer(); return;
    }
    limpiar_buffer();

    printf("Cantidad a agregar al stock de '%s': ", inventario_nombres[sel - 1]);
    if (scanf("%lf", &cant) != 1 || cant <= 0) {
        printf("Cantidad invalida.\n"); limpiar_buffer(); return;
    }
    limpiar_buffer();

    inventario_max[sel - 1] += cant;
    printf("Stock de '%s' actualizado a %.2f unidades.\n", inventario_nombres[sel - 1], inventario_max[sel - 1]);
}
 
void calcular_produccion() {
    int i, j, k, ok = 1;
    double t_tot = 0.0;
    double req_total[TOTAL_INSUMOS_FIJOS];

    for (k = 0; k < TOTAL_INSUMOS_FIJOS; k++) req_total[k] = 0.0;
 
    for (i = 0; i < MAX_PRODUCTOS; i++) {
        if (registrado[i]) {
            t_tot += cantidades[i] * tiempos_unit[i];
            for (j = 0; j < cantidad_tipos_recursos[i]; j++) {
                int idx_inv = buscar_en_inventario(nombres_recursos[i][j]);
                double total_necesitado = cantidades[i] * recursos_unit[i][j];
                
                if (idx_inv != -1) {
                    req_total[idx_inv] += total_necesitado;
                } else {
                    printf("\nALERTA: El insumo '%s' requerido por '%s' no existe en el inventario.", nombres_recursos[i][j], nombres[i]);
                    ok = 0;
                }
            }
        }
    }
 
    printf("\n  EVALUACION DE VIABILIDAD DE PRODUCCION - Dulce Arte");
    printf("\nTiempo total requerido : %.2f / %.2f horas", t_tot, TIEMPO_MAX);
    if (t_tot > TIEMPO_MAX) ok = 0;
    
    printf("\n-------------------------------------------------------");
    for (k = 0; k < TOTAL_INSUMOS_FIJOS; k++) {
        if (req_total[k] > 0) {
            printf("\n%-15s -> Requerido: %6.2f | En Stock: %6.2f", inventario_nombres[k], req_total[k], inventario_max[k]);
            if (req_total[k] > inventario_max[k]) ok = 0;
        }
    }
    printf("\n-------------------------------------------------------");
 
    if (ok) printf("\nRESULTADO: VIABLE - Recursos y tiempo suficientes.\n");
    else printf("\nRESULTADO: NO VIABLE - Revise los excesos de tiempo o stock faltante.\n");
}
 
void listar_todo() {
    int i, j, vacio = 1;
    
    printf("\n  REPORTE COMPLETO - Reposteria Dulce Arte");
    
    printf("\nINVENTARIO CENTRAL DE INSUMOS");
    for (i = 0; i < TOTAL_INSUMOS_FIJOS; i++) {
        printf("\n  * %-15s : %.2f unidades en stock", inventario_nombres[i], inventario_max[i]);
    }
 
    printf("\n\nDETALLE DE PRODUCTOS Y SUS RECETAS");
    for (i = 0; i < MAX_PRODUCTOS; i++) {
        if (registrado[i]) {
            vacio = 0;
            printf("\n\nPRODUCTO: %s", nombres[i]);
            printf("\n  -> Cantidad Solicitada : %d unidades", cantidades[i]);
            printf("\n  -> Tiempo de Prep. Unit: %.2f horas", tiempos_unit[i]);
            printf("\n  -> Receta:");
            for (j = 0; j < cantidad_tipos_recursos[i]; j++) {
                printf("\n     - %.2f de '%s' por unidad", recursos_unit[i][j], nombres_recursos[i][j]);
            }
        }
    }
    if (vacio) printf("\n  [No hay productos registrados]");
    printf("\n=======================================================\n");
}
 
// MAIN
int main() {
    int opcion;
    for (int i = 0; i < MAX_PRODUCTOS; i++) registrado[i] = 0;
    inicializar_inventario();

    printf("\n  Bienvenido al Sistema de Control de Produccion");
    printf("\n  Reposteria \"Dulce Arte\"\n");
 
    do {
        printf("\n=======================================================");
        printf("\n  SISTEMA DE CONTROL DE PRODUCCION - Dulce Arte");
        printf("\n=======================================================");
        printf("\n  1. Ingresar Producto y su Receta");
        printf("\n  2. Editar Producto");
        printf("\n  3. Eliminar Producto");
        printf("\n  4. Verificar Viabilidad de Produccion");
        printf("\n  5. Reabastecer Insumo del Almacen");
        printf("\n  6. Listado General y Reporte");
        printf("\n  0. Salir");
        printf("\n=======================================================");
        printf("\nSeleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) opcion = -1;
        limpiar_buffer();
 
        switch (opcion) {
            case 1: ingresar_producto();      break;
            case 2: editar_producto();        break;
            case 3: eliminar_producto();      break;
            case 4: calcular_produccion();    break;
            case 5: reabastecer_inventario(); break;
            case 6: listar_todo();            break;
            case 0: printf("\nSaliendo del sistema...\n"); break;
            default: printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 0);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Macros de la tabla HASH*/
#define Tam 300
#define VACIO NULL

/*Estrutura que se almacenará en la tabla hash que tendrá:
    - El identificador el elemento
    - El tipo (constante, comando o función)
    - El contenido asociado al tipo (un int o un puntero a una funcion)
*/
typedef struct{
    char* key;
    int tipo;
    void* contenido;
} contStatico;


void InicializarTablaKey();
int BusquedaTablaKey(char *cad, contStatico *e);
void InsertarTablaKey(contStatico e);
void BorrarTablaKey();
void imprimirTablaKey();
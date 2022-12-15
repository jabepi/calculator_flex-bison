#include "../tiposDatos/abb.h"

//Iniciamos la tabla, reservando memoria e introduciendo los elementos inciales
void iniciarTabla();

//Borramos toda la memoria de la tabla
void borrarTabla();

//Insertamos un elemento en la tablaSimbolos
void insertaElemetento(char* id, int nivel, int compLexico);

//Insertamos una variable 
void insertarVariable(char *id, double valor);

//Actualizamos el valor de de una variable en la tabla 
void modificarValorVariable(char *id, double nvalor);

/* Devuelve -1 si no se encuentra el lexema buscado
 * Devuelve el componente léxico que es si se encuentra en la tablaSimbolos*/
void* buscarLexema(char *id, int *tipo);

//Imprimir la tabla de símbolos
void imprimirVariables();

//Añadir la librería de funciones
void addLibrary(char *libreria);

//Buscamos la función en la tabla, y si no existe la buscamos en la librería y la insertamos
void* buscarFuncion(char *nombre);

//Borramos las variables definidas en la tabla
void borrarVariables();


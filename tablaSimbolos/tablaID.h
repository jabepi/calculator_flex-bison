#ifndef IMP_TABLAID_H
#define IMP_TABLAID_H

#include "../tiposDatos/abb.h"

//Iniciamos la tabla de ID
void iniciarTablaID();

//Liberamos la memorias reservada por la tabla de ID
void borrarTablaID();

//Imprimimos todos los elementos de la tabla de ID por niveles
void imprimirTablaID();

//Insertamos un nodo en la tabla
void insertarTablaID(tipoelem nodo);

//Actualizmos un valor en la tabla
void modificarValorTablaID(char *id, double valor);

//Inserta un nodo sen la tabla
int buscarTablaID(char *lexema, tipoelem* nodo);

#endif

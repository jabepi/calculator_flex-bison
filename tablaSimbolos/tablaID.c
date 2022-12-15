#include "tablaID.h"
#include "stdlib.h"

//Variable con el arbol
abb tablaIDs;

/*Iniciamos la tabla de variables*/
void iniciarTablaID() {
    crear(&tablaIDs);
}

//Liberamos la memorias reservada por el bosque
void borrarTablaID() {
    destruir(&tablaIDs);
    free(tablaIDs);
}

//Imprimimos el bosque
void imprimirTablaID() {
    imprimirArbol(tablaIDs);
}

//Insertar un nodo en un nivel concreto del bosque
void insertarTablaID(tipoelem nodo) {
    insertar(&tablaIDs, nodo);
}

//Busca un nodo en la estructura
int buscarTablaID(char *id, tipoelem* nodo) {
    return buscar_nodo(tablaIDs, id, nodo);
}

//Modificamos el valor asociado a un nodo
void modificarValorTablaID(char *id, double valor){
    modificar_val_nodo(tablaIDs, id, valor);
}
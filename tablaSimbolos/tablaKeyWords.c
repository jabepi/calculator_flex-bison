#include "tablaKeyWords.h"
#include "ctype.h"
#include "../definiciones.h"


/*Variable con la tabla hash de keywords*/
contStatico* t[Tam];

/*Funcion Hash*/
int Hash(char *cad) {
    int i, suma = 0;
    int K = 500;
    for (i = (int) strlen(cad) - 1; i >= 0; i--) {
        suma = (suma * K + cad[i]) % Tam;
    }
    return suma;
}

//Crear un obejeto de tipo contStatico
void crearKey(contStatico* nuevo, char *cad, int tipo, void *cont) {
    nuevo = (contStatico*) malloc(sizeof (contStatico));
    nuevo->key = (char*) malloc(sizeof (char) * (strlen(cad) + 1));
    strcpy(nuevo->key, cad);
    nuevo->tipo = tipo;
    nuevo->contenido = cont;
}

/*Devuelve la posisicon de un elemento en la tablaSimbolos*/
int posicion(char *cad) {

    int ini, i, aux;
    ini = Hash(cad);

    for (i = 0; i < Tam; i++) {
        aux = (ini + i * i) % Tam;
        if (t[aux] == VACIO) {
            return aux;
        }
        if (!strcasecmp(t[aux]->key, cad)) {
            return aux;
        }
    }
    return ini;
}


/* Busca en la tabla Hash y devuelve una referencia al símbolo
 (devuelve 1 si esta en la tablaSimbolos y 0 si no esta*/
int BusquedaTablaKey(char *cad, contStatico *e) {
    int pos = posicion(cad);

    if (t[pos] == VACIO)
        return 0;
    else {
        if (strcasecmp(t[pos]->key, cad) == 0) {
            *e = *t[pos];
            return 1;
        } else return 0;
    }
}

/*Inserta un elemento en la tablaSimbolos Hash*/
void InsertarTablaKey(contStatico e) {
    int pos;
    pos = posicion(e.key);

    if (t[pos] == VACIO) {
        t[pos] = malloc(sizeof(contStatico));
        *t[pos] = e;
    } else if (strcmp(t[pos]->key, e.key) != 0) {
        printf("No se encuentra posicion libre.\n");
    }
}

/*Incializamos la tablaSimbolos Hash con las palabras clave*/
void InicializarTablaKey() {

    //**Inicializamos los comando**//
    //Definimos las palabras reservadas
    char *keywords[] = {"QUIT", "HELP", "LOAD", "ECHO",
                        "WORKSPACE", "CLEAR", "FROMFILE"};
    int codigos[] = {QUIT, HELP, LOAD, ECH, WORKSPACE, CLEAR, FROMFILE};
    
    /*Insertamos en la tabla hash las palabras reservadas*/
    for (int i = 0; i < 7; i++) {
        contStatico a;
        a.key = (char *) malloc(sizeof(keywords[i]));
        strcpy(a.key, keywords[i]);
        a.tipo = CMD;
        a.contenido = (int*) malloc(sizeof(int));
        *((int*)(a.contenido)) = codigos[i];
        
        InsertarTablaKey(a);
        
    }

    //**Inicializamos las constantes**//
    //Definimos constantes
    char *constantes[] = {"pi", "e", "fi"};
    double valores[] = {3.14159265358979323846, 2.71828182845904523536, 1.61803398874989484820};

    /*Insertamos en la tabla hash las constantes*/
    for (int i = 0; i < 3; i++) {
        contStatico a;
        a.key = (char *) malloc(sizeof(constantes[i]));
        strcpy(a.key, constantes[i]);
        a.tipo = CONST;
        a.contenido = (double*) malloc(sizeof(double));
        *((double*)(a.contenido)) = valores[i];
        InsertarTablaKey(a);
    }
}


/*Liberamos toda la memoria de la palabras clave*/
void BorrarTablaKey() {
        for(int i= 0; i<Tam; i++){
            if(t[i] != VACIO){
                free(t[i]->key);
                if(t[i]->tipo != FUNCTION){
                    free(t[i]->contenido);
                }
                free(t[i]);
            }
        }
}




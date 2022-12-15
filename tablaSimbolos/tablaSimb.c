#include <dlfcn.h>
#include "tablaSimb.h"
#include "../definiciones.h"
#include "tablaKeyWords.h"
#include "tablaID.h"

void *lib = NULL;

// Reservamosmos memoria para la tabla
void iniciarTabla()
{
    // Inciamos la tabla de ID
    iniciarTablaID();

    // Cargamos las palabras reservada por el lenguaje en la tabla de keywords
    InicializarTablaKey();
}

// Borramos toda la memoria de la tabla
void borrarTabla()
{
    BorrarTablaKey(); // Borramos la memoria de la tabla de palabras reservadas
    borrarTablaID();  // Borramos la memoria de la tabla de ID
    if(lib!=NULL){// Cerramos la libreria
        dlclose(lib);
    }     
}

void imprimirVariables()
{

    printf("------Lista de Variables------\n");
    imprimirTablaID();
    printf("------------------------------\n\n");
}

// Buscamos una variables, un comando o una constante en la estructura
void *buscarLexema(char *lexema, int *tipo)
{

    // Buscamos en la tabla de keyWords
    contStatico id;
    if (BusquedaTablaKey(lexema, &id))
    {
        // Si es una función no existe el elemento en la tabla
        if (id.tipo == FUNCTION)
        {
            return NULL;
        }
        // Si es una constante
        else if (id.tipo == CONST)
        {
            double *valor = (double *)malloc(sizeof(double));
            *((double *)valor) = *((double *)id.contenido);
            *tipo = id.tipo;
            return valor;
        }
        else
        {
            int *valor = (int *)malloc(sizeof(int));
            *(int *)valor = *(int *)id.contenido;
            *tipo = id.tipo;
            return valor;
        }
    }

    // Buscamos el identificador en el árbol
    tipoelem nodo;
    nodo.valor = -1;
    if (buscarTablaID(lexema, &nodo))
    {

        double *valor = (double *)malloc(sizeof(double));
        *(double *)valor = nodo.valor;
        *tipo = VAR;
        return valor;
    }

    // En caso de no encontrarlo lo insertamos en la tabla
    return NULL;
}

// Abrimos la librería
void addLibrary(char *libreria)
{
    if(lib!=NULL){
        printf("Error: Ya se ha cargado una librería\n\n");
        return;
    }
    if (!strcmp(libreria, "math.h"))
    {
        lib = dlopen("libm.so.6", RTLD_NOW);
        if (!lib)
        {
            printf("Error al cargar libreria: no dispone de la libería matemática (libm.so.6)\n\n");
        }
    }
    else
    {
        lib = dlopen(libreria, RTLD_NOW);
        if (!lib)
        {
            printf("Error abriendo la libería: %s", dlerror());
        }
    }
}

// Buscamos una función en la tabla y devolvemos el puntero
void* buscarFuncion(char *nombre)
{
    if(!lib){
        printf("Error: no se ha cargado ninguna libreria\n");
        return NULL;
    }
    contStatico id;
    if (BusquedaTablaKey(nombre, &id))
    {
        if (id.tipo == FUNCTION)
        {
            return id.contenido;
        }
    }
    else
    {
        // Si no está en la tabla la buscamos en la librería y la insertamos
        void *funcion = dlsym(lib, nombre);
        if (funcion)
        {
            contStatico a;
            a.key = (char *)malloc(sizeof(nombre));
            strcpy(a.key, nombre);
            a.tipo = FUNCTION;
            a.contenido = funcion;
            InsertarTablaKey(a);

            return funcion;
        }
    }
    return NULL;
}

// Insertamos/actualizamos el valor de una variable en la tabla
void insertarVariable(char *id, double valor)
{
    // Creamos el nodo
    tipoelem nodo;
    asignarValNodo(&nodo, id, valor);

    // Lo insertamos en el arbol correspondiente (para esta entrega solo trabajamos con un bloque)
    insertarTablaID(nodo);
}

// Actualizamos el valor de de una variable en la tabla
void modificarValorVariable(char *id, double nvalor)
{
    modificarValorTablaID(id, nvalor);
}

//Borrar variables
void borrarVariables()
{
    borrarTablaID();
}
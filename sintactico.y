%{
    #include <stdio.h>
    #include <stdlib.h>

    #include <string.h>
    #include "tablaSimbolos/tablaSimb.h"
    #include "definiciones.h"

    char impresion = 'S';

    extern FILE *yyin;
    extern int yylex_destroy();
    extern void leerDesdeArchivo();
    

    int yylex();  
    int yyparse();
    void yyerror(char *s){
         printf("Error de sintaxis: el comando introducido no es procesable\n\n");
    }; 

    void imprimirInformacion(){
        printf("Información de comandos:\n-QUIT: cierra el programa, y libera toda la memoria.\n-ECHO: muestra/oculta el resutlado de los cálculos por pantalla.\n-LOAD \"nombre librería\": carga una librería para poder utilzar las funciones que posee \n-FROMFILE \"nombre archivo\": permite cargar archivos \".m\" con una lista de comandos separados por saltos de línea y ejecutarlos.\n-WORKSPACE: Imprime las variables que se han definido en la sesión de trabajo.\n-CLEAR: Borra todas las variables definidas en el espacio de trabajo.\n\n");
    }

%}

%union{
    double numero;
    int identificador;
    struct {char* nombre; double valor;}nomVal;
    char* cadena;
}

%token<numero> NUM
%token <cadena>VARNODEF
%token <nomVal> VARIABLE
%token <numero> CONSTANTE
%token <identificador> COMMAND
%token <cadena> FUNCION
%token <cadena> ARG

%left ADD SUB
%left MUL DIV

%token PARENTESIS_OPEN
%token PARENTESIS_CLOSE
%token BRACKET_OPEN 
%token BRACKET_CLOSE
%token SEMICOLON
%token EQUAL
%token EOL
%token COMA

%type <numero> exp 
%type <numero> funcion

%%
    input: 
    |line input
    ;

    line: EOL
        | control EOL 
        | exp SEMICOLON EOL 
        | exp EOL {
            if(impresion == 'S'){
                printf("$> %f\n\n", $1);
            }
        }
        | asignacion EOL 
        | asignacion SEMICOLON EOL 
    ;

    funcion: FUNCION exp PARENTESIS_CLOSE {
        void* aux = buscarFuncion($1);
        free($1);
        if(aux){
            $$ = ((double (*)(double)) aux)($2);
        }else{
            printf("Error de función: función no encotrada\n\n");
        }
    }
    | FUNCION exp COMA exp PARENTESIS_CLOSE{
        void* aux = buscarFuncion($1);
        free($1);
        if(aux){
            $$ = ((double (*)(double, double)) aux)($2, $4);
        }else{
            printf("Error de función: función no encotrada\n\n");
        }
    }
    ;

    exp: funcion {$$ = $1;}
    | VARIABLE {$$ = $1.valor; free($1.nombre);} 
    | VARNODEF {$$ = 0;} 
    | CONSTANTE {$$ = $1;}
    | NUM {$$ = $1;}
    | SUB exp {$$ = -$2;}
    | ADD exp {$$ = $2;}
    | exp ADD exp {$$= $1 + $3;}
    | exp SUB exp {$$= $1 - $3;}
    | exp MUL exp {$$= $1 * $3;}
    | exp DIV exp {$$= $1 / $3;} 
    | PARENTESIS_OPEN exp PARENTESIS_CLOSE {$$= $2;}
    ;

    control: COMMAND ARG {
        if ($1 == LOAD){
            addLibrary($2);
        }else{
            FILE * fichero = fopen($2, "r");
            if(!fichero){
                printf("Error de archivo: no existe el archivo especificado.\n\n");
            }else{
                leerDesdeArchivo(fichero);
            }
        }
        free($2);
        return 1;
    }
    | COMMAND{
        switch($1){
            case QUIT:
                printf("Saliendo del programa\n\n");
                borrarTabla();
                yylex_destroy();
                return 0;
                break;
            case HELP:
                imprimirInformacion();
                break;
            case ECH:
                if(impresion == 'S'){
                    impresion = 'N';
                    printf("Se ha desactivado la impresión por pantalla.\n\n");
                }else{
                    impresion = 'S';
                    printf("Se ha activado la impresión por pantalla.\n\n");
                }
                break;
            case WORKSPACE:
                imprimirVariables();
                break;
            case CLEAR:
                printf("Se han borrado todas las variables\n\n");
                borrarVariables();
                break;
            default:
                printf("Error de comando: el comando no existe o no hay introducidos los argumentos necesarios\n\n");
                break;    
        }
    }
    ;

    asignacion: VARNODEF EQUAL exp {
        //Insertamos la variable en la tabla de símbolos
        insertarVariable($1, $3);
    }
    | VARIABLE EQUAL exp {
        //Actualizamos el valor de la varible en la tabla de símbolos
        modificarValorVariable($1.nombre, $3);
    }
    | CONSTANTE EQUAL exp {
        printf("Error asignacion: No se puede modificar el valor de las constantes\n\n");
    }
    ;

%%


int main(){
    
    //Iniciamos la tabla de símbolos
    imprimirInformacion();
    iniciarTabla();
    while(yyparse());
    return 0;
}
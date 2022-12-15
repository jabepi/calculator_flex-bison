
%{
 #include "sintactico.tab.h"   
 #include "tablaSimbolos/tablaSimb.h"
 #include "definiciones.h"
 #include <stdio.h>

FILE* fichAux;

%}


%option noyywrap
%option yylineno
%option nounput
%option noinput

LETRA               [A-Z]|[a-z]

DECIMAL_DIGT        [0-9]
DECIMAL_DIGTS       {DECIMAL_DIGT}+
DECIMAL_LIT         {DECIMAL_DIGTS}

HEX_DIGT            ([0-9]|[A-F]|[a-f])
HEX_DIGTS           {HEX_DIGT}+
HEX_LIT             0(x|X){HEX_DIGTS}

DECIMAL_EXPONENT    (e|E)[(+|\-)]?{DECIMAL_DIGTS}

FLOAT_LIT1          {DECIMAL_DIGTS}"."{DECIMAL_DIGT}*({DECIMAL_EXPONENT})?
FLOAT_LIT2          {DECIMAL_DIGTS}{DECIMAL_EXPONENT}
FLOAT_LIT3          "."{DECIMAL_DIGTS}({DECIMAL_EXPONENT})?

ID                  ({LETRA})({LETRA}|{DECIMAL_DIGT})*

ARG                 {ID}"."("h"|"so"|"dylib"|"lib"|"m")

FUNC                ({LETRA})({LETRA}|{DECIMAL_DIGT})*("(")
INT_LIT             ({DECIMAL_LIT}|{HEX_LIT})
FLOAT_LIT           {FLOAT_LIT1}|{FLOAT_LIT2}|{FLOAT_LIT3}

CARACT_IGN          [ \t]+

%%

{CARACT_IGN}

{FLOAT_LIT}|{INT_LIT}           {yylval.numero = atof(yytext); 
                                return NUM;}

{FUNC}                          {
                                    int tam = strlen(yytext);
                                    yylval.cadena = malloc(sizeof(char)*(tam));
                                    strncpy(yylval.cadena, yytext, tam-1);
                                    yylval.cadena[tam-1]= '\0'; 
                                    return FUNCION;
                                    
                                }

{ID}                            {int tipo = 0, tam;
                                void *valor = NULL;
                                
                                /*Vamos a la tabla de símbolos para determinar si es
                                un variable, una constante, o un comando*/
                                valor = buscarLexema(yytext, &tipo);

                                //Si se encontraba en la tabla
                                if(valor!=NULL){
                                    switch(tipo){
                                        case VAR:
                                            //Enviamos el nombre de la variable
                                            tam = strlen(yytext);
                                            yylval.nomVal.nombre = malloc(sizeof(char)*(tam+1));
                                            strncpy(yylval.nomVal.nombre, yytext, tam);
                                            yylval.nomVal.nombre[tam]='\0';
                                            
                                            //Enviamos el valor asociado en la tabla
                                            yylval.nomVal.valor = *((double*)valor);
                                            free(valor);
                                            
                                            return VARIABLE;
                                        case CONST:
                                            yylval.numero = *((double*)valor);
                                            free(valor);
                    
                                            return CONSTANTE;
                                        case CMD:
                                            yylval.identificador = *((int*)valor);
                                            free(valor);
                                            return COMMAND;
                                    }
                                }
                                //Si no estaba, es una variable no definida
                                else{
                                    tam = strlen(yytext);
                                    yylval.cadena = malloc(sizeof(char)*(tam+1));
                                    strncpy(yylval.cadena, yytext, tam);
                                    yylval.cadena[tam]= '\0'; 
                                    return VARNODEF;
                                }
                                }

{ARG}                           {int tam = strlen(yytext);
                                yylval.cadena = malloc(sizeof(char)*(tam+1));
                                strncpy(yylval.cadena, yytext, tam);
                                yylval.cadena[tam]= '\0'; 
                                return ARG;}

\n                              {return EOL;}

"+"                 {return ADD;}
"-"                 {return SUB;}
"/"                 {return DIV;} 
"*"                 {return MUL;}

")"                 {return PARENTESIS_CLOSE;}
"("                 {return PARENTESIS_OPEN;}

";"                 {return SEMICOLON;}
"="                 {return EQUAL;}
"["                 {return BRACKET_OPEN;}
"]"                 {return BRACKET_CLOSE;}

","                 {return COMA;}

<<EOF>>             {fclose(yyin);yypop_buffer_state();}

.                   


%%

void leerDesdeArchivo(FILE* fichero){
    fichAux = fichero;
    yyin = fichero;
    yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
}




#!/bin/bash
flex -l lex.l && \
bison -dv sintactico.y && \
gcc -Wall -o calculadora lex.yy.c sintactico.tab.c tablaSimbolos/*.c tiposDatos/*.c -ldl




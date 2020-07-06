%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "token.h"
	int line = 0;


%}
digit             [0-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit})
hex_digit         [0-9A-F]
identifier        {alpha}{alpha_num}*
unsigned_integer  {digit}+
hex_integer       ${hex_digit}{hex_digit}*
exponent          e[+-]?{digit}+
i                 {unsigned_integer}
real              ({i}\.{i}?|{i}?\.{i}){exponent}?
string            \'([^'\n]|\'\')+\'

%%
<<EOF>> {return EXIT;}
"\n" {line+=1;}
"\n"|"\t"|" " { }
":=" {return ASSIGNMENT;}
"+" {return ADITTION;}
"-" {return SUBTRACTION;}
"*" {return MULTIPLICATION;}
"/" {return REAL_DIVISION;}
"div" {return INTEGER_DIVISION;}
"mod" {return INTEGER_MODULUS;}
"=" {return EQ;}
"<>" {return UEQ;}
"<" {return LE;}
">" {return GE;}
"<=" {return LEQ;}
">=" {return GEQ;}
"not" {return NOT;}
"or" {return OR;}
"and" {return AND;}
"(" {return LPARENTHESES;}
")" {return RPARENTHESES;}
"["|"]" {return COMMENT_BRACKET;}
"("|")" {return COMMENT_BRACKET;}
"," {return COMMA;}
"." {return POINT;}
";" {return SEMICOLON;}
":" {return COLON;}
"^" {return HAT;}
"and" {return AND;}
"array" {return ARRAY;}
"begin" {return begin;}
"case" {return CASE;}
"const" {return CONST;}
"do" {return DO;}
"downto" {return DOWNTO;}
"else" {return ELSE;}
"end" {return END;}
"file" {return file;}
"for" {return FOR;}
"function" {return FUNCTION;}
"goto" {return GOTO;}
"if" {return IF;}
"in" {return IN;}
"label" {return LABEL;}
"nil" {return NIL;}
"of" {return OF;}
"packed" {return PACKED;}
"procedure" {return PROCEDURE;}
"program" {return PROGRAM;}
"record" {return RECORD;}
"repeat" {return REPEAT;}
"set" {return SET;}
"then" {return THEN;}
"to" {return TO;}
"type" {return TYPE;}
"until" {return UNTIL;}
"var" {return VAR;}
"while" {return WHILE;}
"with" {return WITH;}
{unsigned_integer}   return(CONST);
{real}               return(CONST);
{hex_integer}        return(CONST);
{identifier} return(IDENTIFIER);
{string}             return(CONST);

.                    {printf("LEXICAL ERROR\nInvalid token = %s in line %i\n",Scannertext, line); exit(1);}
%%


int Scannerwrap(void){return 1;}


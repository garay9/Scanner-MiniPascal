%{
	#include <stdio.h>
	#include "token.h"

%}

digit             [0-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit})
hex_digit         [0-9A-F]
identifier        [a-zA-Z][a-zA-Z0-9]*
unsigned_integer  {digit}+
hex_integer       ${hex_digit}{hex_digit}*
exponent          e[+-]?{digit}+
i                 {unsigned_integer}
real              ({i}\.{i}?|{i}?\.{i}){exponent}?
string            \'([^'\n]|\'\')+\'


%%
%caseless
<<EOF>> {return EXIT;}
[(][*][^*]*[*]+([^*)][^*]*[*]+)*[)] { }
[{][^}]*[}] { }


"#include"(" "|"\n"|"\t")*"<"[a-zA-Z][a-zA-Z0-9_]*"."[a-zA-Z_][a-zA-Z_]*">" {return INCLUDE;}
"#define"(" "|"\t")+{identifier}+(" "|"\t")+((({digit}|{real}|{identifier})+(("+"|"-"|"/"|"div"|"mod")({real}|{identifier}|{digit})+)+)+|({string}|{alpha}|{digit}|{identifier}|{real})+) {return DEFINE;}
{identifier} return(IDENTIFIER);
."\n"? {return 0;}
%%
int Preprocwrap(void){return 1;}

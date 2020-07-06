#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

extern char* getTokenName(int a, int add);
extern struct tableInt *createTableInt(int size);
extern int hashCodeInt(struct tableInt *t,int key);
extern void insertInt(struct tableInt *t,int key,int val);
extern int lookupInt(struct tableInt *t,int key);
extern struct tableChar *createTableChar(int size);
extern int hashCodeChar(struct tableInt *t,int key);
extern void insertChar(struct tableChar *t,int key,char val[127]);
extern char* lookupChar(struct tableChar *t,int key);


FILE *fp;
typedef char string[1024];
void makePortada(FILE *fp){
	fputs("\\documentclass[dvipsnames]{beamer}",fp);
	fputs("\n\\usepackage{graphicx}",fp);
    fputs("\n\\usepackage{pgf-pie}",fp);
	fputs("\n\\usepackage{float}",fp);
	fputs("\n\\usepackage{tabularx}",fp);
    fputs("\n\\usepackage{xcolor}",fp);
    fputs("\n\\usepackage{pgfplots}",fp);
    fputs("\n\\pgfplotsset{compat=1.15}", fp);
	fputs("\n\\usepackage[english]{babel}\n",fp);
	fputs("\n\\usepackage[utf8x]{inputenc}\n",fp);
    fputs("\\usetheme{Madrid}",fp);
	fputs("\n\\title{Proyecto 1\\\\ \\small{Scanner con Flex}\\\\\\small{ Compiladores e Intérpretes $\\mid$ IC5701}\\\\Instituto Tecnológico de Costa Rica.}", fp);
	fputs("\n\\author{Jorge Blanco, David Martínez, Daniel Vargas}",fp);
	fputs("\n\\date{I Semestre 2020}",fp);
	fputs("\\onecolumn\n", fp);
	fputs("\n\\begin{document}\\maketitle", fp);
	fputs("\n\\newcommand{\\C}[1]{\\texttt{#1}}", fp);
	fputs("\n\\newcommand{\\B}[1]{\\textbf {#1}}", fp);
	fputs("\n\\newcommand{\\X}[1]{\\textbf {\\underline{#1}}}\n", fp);
	
	//
}

void addImage(string archivo, string texto){
    fputs("\n\\begin{figure}[ht]", fp);
    fputs("\n\\caption{", fp);
    fputs(texto,fp);
    fputs("}",fp);
    fputs("\n\\centering\\includegraphics[width=10cm]{", fp);
    fputs(archivo,fp);
    fputs("}",fp);
    fputs("\n\\end{figure}", fp);


}

void makeIntroduccion(FILE *fp){
    fputs("\n\\begin{frame}", fp);
    fputs("\n\\frametitle{Scanner/Analizador Léxico}", fp);
    fputs("El análisis léxico es el proceso de la conversión de caracteres en \"tokens\" mediante el análisis de un archivo fuente y una definición de significados de strings que corresponden a un token(lexema)",fp);
    addImage("imagenes/lexico", "Análisis Léxico de un compilador");
    fputs("\n\\end{frame}", fp);
    fputs("\n\\begin{frame}", fp);
    fputs("\n\\frametitle{Flex}", fp);
    fputs("\nFlex  es  una  herramienta  de  los  sistemas  UNIX/Linux  que permite  generar un scanner con código C. Flex permite asociar acciones descritas en C,  a  la  localización  de  las  Expresiones  Regulares  que  le  hayamos  definido, es decir, podemos realizar acciones específicas a partir de los ¨Tokens¨ que definamos, esto mediante una plantilla diseñada por el usuario en la que se basará Flex.",fp);
    addImage("imagenes/flex", "Funcionamiento de Flex");
    fputs("\n\\end{frame}", fp);
    fputs("\n\\begin{frame}", fp);
    fputs("\n\\frametitle{Scanner}", fp);
    fputs("Internamente Flex va a actuar como un autómata que localizará las expresiones regulares descritas en la plantilla y al encontrar dicha expresion ejecutará el código asociado a ella.", fp);
    addImage("imagenes/automata", "Máquina de estados finitos");
    fputs("\n\\end{frame}", fp);
}    

void makeTokens(FILE *fp, struct tableInt* tokens, struct tableChar* lexemas){
    fputs("\n\\begin{frame}", fp);
    fputs("\n\\frametitle{Tokens}", fp);
    fputs("\n\\textcolor{White}{",fp);
    int newSlide = 1;
    for(int i = 0; strcmp(lookupChar(lexemas, i), ""); i++){
        if(i >= 50 && newSlide){
            newSlide = 0;
            fputs("}",fp);
            fputs("\n\\end{frame}", fp);
            fputs("\n\\begin{frame}", fp);
            fputs("\n\\frametitle{Tokens}", fp);
            fputs("\n\\textcolor{White}{",fp);
        }
        int token = lookupInt(tokens, i);
        char* lexema = lookupChar(lexemas, i);
        char codigo[100] = "";
        
        if(token >= 0 && token <= 6){
            strcpy(codigo, "\\colorbox{Turquoise}{");
            strcat(codigo, lexema);
            strcat(codigo, "} ");
            fputs(codigo,fp);
        }else if(token >= 7 && token <= 13){
            strcpy(codigo, "\\colorbox{Bittersweet}{");
            strcat(codigo, lexema);
            strcat(codigo, "} ");
            fputs(codigo,fp);
        }else if(token >= 14 && token <= 16){
            strcpy(codigo, "\\colorbox{Apricot}{");
            strcat(codigo, lexema);
            strcat(codigo, "} ");
            fputs(codigo,fp);
        }else if(token >= 17 && token <= 20){
            strcpy(codigo, "\\colorbox{Orchid}{");
            strcat(codigo, lexema);
            strcat(codigo, "} ");
            fputs(codigo,fp);
        }else if(token >= 21 && token <= 26){
            strcpy(codigo, "\\colorbox{LimeGreen}{");
            strcat(codigo, lexema);
            strcat(codigo, "} ");            
            fputs(codigo,fp);
            if(token == 23){
                fputs("\\\\", fp);
            }
        }else {
            if(token == 58){
                strcpy(codigo, "\\colorbox{Goldenrod}{");
            }else if(token == 29){
                strcpy(codigo, "\\colorbox{NavyBlue}{");
            }else{
                strcpy(codigo, "\\colorbox{RoyalPurple}{");
            }
            
            strcat(codigo, lexema);
            strcat(codigo, "} ");
            fputs(codigo,fp);
            
            //else || then || begin
            if(token == 27 || token == 32 || token == 48){
                fputs("\\\\", fp);
            }
        }
        

    }
    fputs("}",fp);
    fputs("\n\\end{frame}", fp);
}

void makeHistograma(FILE *fp, struct tableInt* mapa){
    fputs("\n\\begin{frame}", fp);
    fputs("\\frametitle{Histograma de ocurrencia de Tokens}", fp);
    fputs("\n\\begin{tikzpicture}",fp);
    fputs("\n\\begin{axis}[ybar interval, ylabel= Cant.,xlabel= Token,symbolic x coords={",fp);
    int cantTokens = 0;
    if(lookupInt(mapa, 0) > 0){
        fputs(getTokenName(0, 0), fp);
        cantTokens++;
    }
    for(int i = 1; cantTokens <= 7; i++){
        if(lookupInt(mapa, i) > 0){
            fputs(", ", fp);
            fputs(getTokenName(i, 0), fp);
            cantTokens++;
        }
    }
    cantTokens = 0;
    fputs(", ", fp);
    fputs("xd", fp);
    fputs("}]",fp);
    fputs("\n\\addplot coordinates {",fp);
    for(int i = 0; cantTokens <= 7; i++){
        
        char str[10] = "";
        int tokenCount = lookupInt(mapa, i);
        if(tokenCount > 0){            
            sprintf(str, "%d", tokenCount);
            fputs("(",fp);
            fputs(getTokenName(i, 0),fp);
            fputs(", ",fp);
            fputs(str,fp);
            fputs(") ",fp);
            cantTokens++;
        }
        
    }
    fputs("(xd, 0)", fp);
    fputs("};",fp);    
    fputs("\n\\end{axis}",fp);
    fputs("\n\\end{tikzpicture}",fp);
    fputs("\n\\end{frame}", fp);

}

void makePastel(FILE *fp, struct tableInt* mapa){
    fputs("\n\\begin{frame}", fp);
    fputs("\\frametitle{Distribución de Tokens}", fp);
    fputs("\n\\begin{tikzpicture}",fp);
    fputs("\n\\pie[]{",fp);
    int totalTokens = 0;
    for(int i = 0; i < 58; i++){
        totalTokens += lookupInt(mapa, i);
    }
    int exit;
    double porcentajesPequenos = 0;
    for(int i = 0; i < 58; i++){
        char str[1024] = "";   
        int tokenCount = lookupInt(mapa,i);     
        if(tokenCount > 0){
            double porcentaje = tokenCount / (double)totalTokens;
            porcentaje = porcentaje*100;
            if(porcentaje < 2){
                porcentajesPequenos += porcentaje;
            }else{
                sprintf(str, "\n  %.2f", porcentaje);
                fputs(str,fp);
                fputs("/  ",fp);
                fputs(getTokenName(i,0),fp);
            }
            exit = i;
            break;
        }
    }
       for(int i = exit+1; i < 58; i++){
        char str[10] = "";   
        int tokenCount = lookupInt(mapa,i); 
        if(tokenCount > 0){    
            double porcentaje = tokenCount / (double)totalTokens;
            porcentaje = porcentaje*100;
            if(porcentaje < 2){
                porcentajesPequenos += porcentaje;
            }else{
                sprintf(str, "%.2f", porcentaje);
                fputs(",\n",fp);
                fputs(str,fp);
                fputs("/  ",fp);
                fputs(getTokenName(i,0),fp);
            }
        }
    }
    char str[10] = "";
    if(porcentajesPequenos >= 0.01){
        sprintf(str, "%.2f", porcentajesPequenos);
        fputs(",\n",fp);
        fputs(str,fp);
        fputs("/  ",fp);
        fputs("Otros",fp);        
    }
    fputs("}",fp);
    fputs("\n\\end{tikzpicture}",fp);
    fputs("\n\\end{frame}", fp);
    

}

void compileTEX(char source[12]){
	char compile[50] = "";
	strcpy(compile, "pdflatex ");
	strcat(compile, source);
	system(compile);
}

void openPDF(char pdf[12]){
	char open[50] = "";
	strcpy(open, "evince --presentation ");
	strcat(open, pdf);
	int estado = system(open);
	
}


void Latex(struct tableInt* mapa, struct tableInt* tokens, struct tableChar* lexemas){
    
    char source[12] = "scanner.tex";
    char pdf[12] = "scanner.pdf";   
    fp = fopen(source, "w+");
    makePortada(fp);
    makeIntroduccion(fp);
    
    makeTokens(fp, tokens, lexemas);
    
    makeHistograma(fp, mapa);
    
    makePastel(fp, mapa);
    
    fputs("\n\\end{document}", fp);
	fclose(fp);
    compileTEX(source);
	openPDF(pdf);
}

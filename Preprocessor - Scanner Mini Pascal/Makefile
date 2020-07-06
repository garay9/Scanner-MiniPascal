all: gary

lex.Scanner.c: scanner.l
	flex -PScanner scanner.l

lex.Preproc.c: flexPreprocessor.l
	flex -PPreproc flexPreprocessor.l

gary: Preprocessor.c
	gcc Preprocessor.c hashMapInt.c hashMapChar.c lex.Preproc.c lex.Scanner.c Scanner.c generadorLatex.c -o main

Preprocessor.c: hashMapInt.c hashMapChar.c lex.Preproc.c lex.Scanner.c Scanner.c generadorLatex.c

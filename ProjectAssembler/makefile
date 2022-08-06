mymat : mymat.o mat.o
	gcc -g -ansi -Wall -pedantic mymat.o mat.o -o mymat

directives.o: main.c mat.h
	gcc -c -ansi -Wall -pedantic directives.c -o directives.o

code_parse.o: code_parse.c code_prse.h
	gcc -c -ansi -Wall -pedantic code_parse.c -o code_parse.o
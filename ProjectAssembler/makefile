assembler : main.o directives.o code_parse.o symbol_table.o symbol_conversion.o orders.o label_check.o string_utils.o utils.o macro.o
	gcc -g -ansi -Wall -pedantic main.o directives.o code_parse.o symbol_table.o symbol_conversion.o orders.o label_check.o string_utils.o utils.o macro.o -lm -o assembler

main.o : main.c directives.h code_parse.h symbol_table.h symbol_conversion.h orders.h label_check.h string_utils.h utils.h macro.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

directives.o: directives.c directives.h utils.h
	gcc -c -ansi -Wall -pedantic directives.c -o directives.o

code_parse.o: code_parse.c code_parse.h
	gcc -c -ansi -Wall -pedantic code_parse.c -o code_parse.o

utils.o : utils.c utils.h 
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o

symbol_table.o: symbol_table.c symbol_table.h
	gcc -c -ansi -Wall -pedantic symbol_table.c -o symbol_table.o

symbol_conversion.o: symbol_conversion.c symbol_conversion.h
	gcc -c -ansi -Wall -pedantic symbol_conversion.c -o symbol_conversion.o

string_utils.o : string_utils.c string_utils.h 
	gcc -c -ansi -Wall -pedantic string_utils.c -o string_utils.o

orders.o : orders.c orders.h utils.h
	gcc -c -ansi -Wall -pedantic orders.c -o orders.o

label_check.o : label_check.c label_check.h utils.h
	gcc -c -ansi -Wall -pedantic label_check.c -o label_check.o

macro.o : macro.c macro.h 
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o

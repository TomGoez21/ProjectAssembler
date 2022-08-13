#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ctype.h> 
#include "utils.h"
#include "code_parse.h"
#include "symbol_conversion.h"
#include "write_to_file.h"

#pragma warning(disable : 4996)


void write_code_table_to_file(CodeTable* code_table, char* filename) {
	int i;
	FILE* new_file;
	/*change to .ob extension in filename*/
	filename[strlen(filename) - 2] = 'o';
	filename[strlen(filename) - 1] = 'b';
	new_file = fopen(filename, "w");

	/*checks if openning file succedded*/
	if (new_file == NULL) {
		printf("Can't write to  %s", filename);
		return;
	}
	/* if code table is empty, quit */
	if (code_table == NULL) {
		return;
	}
	/*write all code to file*/
	fprintf(new_file, "m     f");
	for (i=0; i < code_table->size; i++) {
		fprintf(new_file, "\n%s     %s", decimalToSymbolsBase(code_table->entries[i].address), binToSymbolsBase(code_table->entries[i]. code));
	}
	fclose(new_file);
	return;
}
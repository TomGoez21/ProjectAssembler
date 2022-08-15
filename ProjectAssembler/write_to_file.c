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
		fprintf(stderr, "Can't write to  %s", filename);
		set_error(true);
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


void write_to_extern_file(char* label, int* address, char* ext_filename) {
	FILE* new_file;

	new_file = fopen(ext_filename, "a");

	/*checks if openning file succedded*/
	if (new_file == NULL) {
		fprintf(stderr, "Can't write to  %s", ext_filename);
		set_error(true);
	}

	/*write extern label to file*/
	fprintf(new_file, "%s     %s\n", label, decimalToSymbolsBase(*address));
	fclose(new_file);
	return;
}



void write_to_entry_file(SymbolTable* symboltable, char* filename) {
	int i;
	int j;
	FILE* new_file;
	/*change to .ob extension in filename*/
	char* entry_filename = calloc(strlen(filename) + 2, sizeof(char));
	strcpy(entry_filename, filename);
	entry_filename[strlen(filename) - 2] = 'e';
	entry_filename[strlen(filename) - 1] = 'n';
	entry_filename[strlen(filename)] = 't';
	entry_filename[strlen(filename) + 1] = '\0';

	new_file = fopen(entry_filename, "w");

	/*checks if openning file succedded*/
	if (new_file == NULL) {
		fprintf(stderr, "Can't write to  %s", entry_filename);
		set_error(true);
	}
	/* if code table is empty, quit */
	if (symboltable == NULL) {
		return;
	}
	/*write all entry labels to file*/
	for (i = 0; i < symboltable->size; i++) {
		if (symboltable->entries[i].type == _ENTRY) {
			for (j = 0; j < symboltable->size; j++) {
				if (symboltable->entries[j].type == _ENTRY) {
					continue;
				}

				if (strcmp(symboltable->entries[i].symbol_name, symboltable->entries[j].symbol_name) == 0) {
					fprintf(new_file, "\n%s     %s", symboltable->entries[j].symbol_name , decimalToSymbolsBase(symboltable->entries[j].counter));
				}
			}
		}
	}
	fclose(new_file);
	return;
}
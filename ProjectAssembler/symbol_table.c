#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symbol_table.h"



bool label_exists(SymbolTable* table, SymbolTableEntry* to_add) {
	int i;
	for (i = 0; i < table->size; ++i) {
		if (strcmp(table->entries[i].symbol_name, to_add->symbol_name) == 0) {
			return true;
		}
	}
	return false;
}

void add_to_table(SymbolTable* table, SymbolTableEntry to_add) {
	if (label_exists(table, &to_add)) {
		/*TODO: print row numberand indicitive error*/ 
	}
	table->size++;
	if (!table->entries) {
		/*First add :*/ 
		table->entries = (SymbolTableEntry*)malloc(table->size * sizeof(SymbolTableEntry)); /* Initial allocation */
	}
	else {
		/* Allocate more memory for the new entry */
		table->entries = (SymbolTableEntry*)realloc(table->entries, table->size * sizeof(SymbolTableEntry));
	}
	if (!table->entries) {
		printf("Memory allocation failed.");
		exit(1);
	}
	/* (Shallow) copy the new entry to the table*/
	memcpy(&(table->entries[table->size - 1]), &to_add, sizeof(SymbolTableEntry));
}


SymbolTableEntry* find_label_from_table(SymbolTable* table, char* label) {
	int i;
	for (i = 0; i < table->size; ++i) {
		if (strcmp(table->entries[i].symbol_name, label) == 0) {
			return &table->entries[i];
		}
	}
}
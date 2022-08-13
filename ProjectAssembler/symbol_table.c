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
	if (label_exists(table, &to_add) && to_add.type == _ENTRY) {
		fprintf(stderr, "label %s already exists in symbol table\n", to_add.symbol_name);
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
		fprintf(stderr, "Memory allocation failed\n");
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

void freeSymbolTable(SymbolTable* table) {
	int i;
	SymbolTableEntry*  tmp = NULL;
	char* chTmp = NULL;
	for (i = 0; i < table->size; ++i) {
		tmp = &(table->entries[i]);
		chTmp = tmp->symbol_name;
		free(chTmp);
	}
	free(table->entries);
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symbol_table.h"

void add_to_table(SymbolTable* table, SymbolTableEntry to_add) {
	table->size++;
	/* Allocate more memory for the new entry */
	table->entries = realloc(table->entries, table->size * sizeof(SymbolTableEntry));
	/* (Shallow) copy the new entry to the table*/
	memcpy(&(table->entries[table->size - 1]), &to_add, sizeof(SymbolTableEntry));
}
/*check symbol doesnt already exsit*/



SymbolTableEntry construct_entry(char* text, int IC, int DC) {

}
#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_
#include <stdbool.h>

typedef enum label_type {
	_DATA, _CODE, _EXTERN, _ENTRY, _NO_TYPE
} label_type;

typedef struct SymbolTableEntry {
	char* symbol_name;
	int counter;
	label_type type;
} SymbolTableEntry;


typedef struct SymbolTable {
	SymbolTableEntry* entries;
	int size;
} SymbolTable;

void add_to_table(SymbolTable* table, SymbolTableEntry to_add);

SymbolTableEntry* find_label_from_table(SymbolTable* table, char* label);

bool label_exists(SymbolTable* table, char* symbol_name);

#endif
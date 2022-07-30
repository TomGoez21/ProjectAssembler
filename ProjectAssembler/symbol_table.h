#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_


typedef enum label_type {
	_DATA, _CODE, _EXTERN, _ENTRY
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


#endif
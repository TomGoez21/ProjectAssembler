#ifndef WRITE_TO_FILE_H_
#define WRITE_TO_FILE_H_

#include "code_parse.h"
#include "symbol_table.h"

void write_code_table_to_file(CodeTable* code_table, char* filename);
void write_to_extern_file(char* label, int* address, char* ext_filename);
void write_to_entry_file(SymbolTable* symboltable, char* filename);

#endif
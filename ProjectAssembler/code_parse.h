#ifndef CODE_PARSE_H_
#define CODE_PARSE_H_

#include "utils.h"
#include "symbol_table.h"

typedef enum adderssing_type {
	IMMEDIATE_ADD, DIRECT_ADD, STRUCT_ADD, REGISTER_ADD, NONE
} addressing_type;


/* code word structure */
typedef struct code_structure {
	unsigned int opcode : 4;
	/*address - for immediate or label addressing*/
	int address : 8;
	unsigned int src_reg_add : 4;
	unsigned int dst_reg_add : 4;
	unsigned int src_addressing : 2;
	unsigned int dest_addressing : 2;
	unsigned int ARE : 2;
} code_structure;


typedef struct CodeTableEntry {
	char* code;
	int address;
} CodeTableEntry;


typedef struct CodeTable {
	CodeTableEntry* entries;
	int size;
} CodeTable;

void add_to_code_table(CodeTable* table, CodeTableEntry to_add);

int operands_check(line_details line, long* code_image_ptr, long *IC);

addressing_type parse_operand_addressing_type(long* L, line_details line, char* operand, long code_image_ptr[][80], long* IC);

void validate_operand_addressing(char** oper, long* L_ptr, line_details line, addressing_type* src_address, addressing_type* dst_address, char** src_oper, char** dst_oper, long code_image_ptr[][80], long* IC);

void opcode_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line);

void src_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable, char* extern_filename);

void dst_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable, char* extern_filename);

void data_image_to_code_table(long** data_image_ptr, CodeTable* codetable, long* IC, long* DC);

#endif
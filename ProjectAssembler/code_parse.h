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

/*
 * Function:  add_to_code_table
 * --------------------
 * add CodeTableEntry struct to CodeTable
 *
 * input:	CodeTable*
 *			CodeTableEntry to_add
 */
void add_to_code_table(CodeTable* table, CodeTableEntry to_add);


/*
 * Function:  parse_operand_addressing_type
 * --------------------
 * determine the type of addressing. also add to the word counter L*
 *
 * input:	long* L
 *			line_details line (struct for line info)
 *			char* operand
 *			long code_image_ptr[][80]
 *			long* IC - image counter
 *
 */
addressing_type parse_operand_addressing_type(long* L, line_details line, char* operand, long code_image_ptr[][80], long* IC);

int operands_check(line_details line, long* code_image_ptr, long* IC);

/*
 * Function:  validate_operand_addressing
 * --------------------
 * determine the type of addressing. also add to the word counter L*
 *
 * input:	long* L
 *			line_details line (struct for line info)
 *			addressing_type* src_address
 *			addressing_type* dst_address
 *			char* operand
 *			long code_image_ptr[][80]
 *			long* IC - image counter
 *
 */
void validate_operand_addressing(char** oper, long* L_ptr, line_details line, addressing_type* src_address, addressing_type* dst_address, char** src_oper, char** dst_oper, long code_image_ptr[][80], long* IC);



/*
 * Function:  opcode_to_bin
 * --------------------
 * check the type of opscode thereafter parsing the 2 operands
 *
 * input:	long* L
 *			line_details line (struct for line info)
 *			addressing_type* src_address
 *			addressing_type* dst_address
 *			char* operand
 *			CodeTable* codetable
 *			CodeTableEntry* code_table_line
 *
 */
void opcode_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line);




/*
 * Function:  src_to_bin
 * --------------------
 * convert the src_oper to binary and store it on the code_table_line(struct)
 *
 * input:	long* L
 *			line_details line (struct for line info)
 *			addressing_type* src_address
 *			addressing_type* dst_address
 *			char* src_oper
 *			char* dst_oper
 *			CodeTable* codetable
 *			CodeTableEntry* code_table_line
 *
 */
void src_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable, char* extern_filename);


/*
 * Function:  dst_to_bin
 * --------------------
 * convert the src_oper to binary and store it on the code_table_line(struct)
 *
 * input:	long* L
 *			line_details line (struct for line info)
 *			addressing_type* src_address
 *			addressing_type* dst_address
 *			char* src_oper
 *			char* dst_oper
 *			CodeTable* codetable
 *			CodeTableEntry* code_table_line
 *
 */
void dst_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable, char* extern_filename);


/*
 * Function:  data_image_to_code_table
 * --------------------
 * adds all data_image to the code table with address and binary conversion
 *
 * input:
 *			long** data_image_ptr
 *			CodeTable* codetable
 *			long* IC - Image counter
 *			long* DC - Data counter
 *
 */
void data_image_to_code_table(long** data_image_ptr, CodeTable* codetable, long* IC, long* DC);

/*
 * Function:  freeCodeTable
 * --------------------
 * free the table memorys allocation
 *
 * input:
			CodeTable* codetable
 *
 */
void freeCodeTable(CodeTable* codetable);

void src_dst_reg_to_bin(long* L_ptr, long* IC, char* opernad, addressing_type src_add, addressing_type dst_add, char* src_oper, char* dst_oper, CodeTable* codetable, CodeTableEntry* code_table_line, SymbolTable* symboltable);
#endif
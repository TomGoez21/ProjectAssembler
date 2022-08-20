#ifndef LABEL_CHECK_H_
#define LABEL_CHECK_H_
#define MAX_LABEL_LENGTH 30

#include "code_parse.h"


/*
 * Function:  get_order_num
 * --------------------
 * gets the decimal value of an order
 *
 * input:	char* order
 *
 * Returns: the number of the order from the order_list
 */
int get_order_num(char* order);


/*
 * Function:  is_order
 * --------------------
 * check if text is one of the orders from order_list
 *
 * input:	line_details line
 *
 * Returns: 1 if text equals to order from order_list
 */
bool is_order(line_details line);

void check_src_dst_per_opcode(char* opcode, addressing_type src_add, addressing_type dst_add, line_details line);



/*
 * Function:  is_reserved_word
 * --------------------
 * check if text is one of the orders or directive or registers.
 *
 * input:	line_details line
 *			char* text - the curr line
 * 
 * Returns: 1 if is reserved word
 */
bool is_reserved_word(line_details line, char* text);




/*
 * Function:  is_label_valid
 * --------------------
 *  Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits
 *
 * input:	line_details line
 *			char* text - the curr line
 *			SymbolTable* symboltable - pointer to symbol table
 *			is_second_run - tells if it is the second file pass
 *
 * Returns: 1 if text is a valid label
 */
bool is_label_valid(line_details line, char* text, SymbolTable* symboltable, bool is_second_run);


/*
 * Function:  is_label_valid_in_struct
 * --------------------
 * Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits
 *
 * input:	line_details line
 *			char* text - the curr line
 *
 * Returns: 1 if valid label in struct
 */
bool is_label_valid(line_details line, char* text, SymbolTable* symboltable, bool is_second_run);
bool is_label_valid_in_struct(line_details line, char* text);



/*
 * Function:  is_label_valid
 * --------------------
 * Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits
 *
 * input:	line_details line
 *			char* text - the curr line
 *
 * Returns: 1 if valid label in text
 */
bool is_label_valid_in_text(line_details line, char* text);




/*
 * Function:  get_label
 * --------------------
 * Get the label in current line. 
 *
 * input:	line_details line
 *			char* text - the curr line
 *			SymbolTable* symboltable - pointer to symbol table
 *			is_second_run - tells if it is the second file pass
 *
 * Returns: pointer to label.
 */
char* get_label(line_details line, SymbolTable* symboltable, bool is_second_run);


char* get_label_in_struct(char* text, char* label_name);
#endif /* LABEL_CHECK_H_ */

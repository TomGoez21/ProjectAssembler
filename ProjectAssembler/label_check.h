#ifndef LABEL_CHECK_H_
#define LABEL_CHECK_H_
#define MAX_LABEL_LENGTH 30

#include "code_parse.h"

char* get_order(line_details line);
int* get_order_num(char* order);
bool is_order(line_details line);
void check_src_dst_per_opcode(char* opcode, addressing_type src_add, addressing_type dst_add, line_details line);
bool is_reserved_word(line_details line, char* text);
bool is_label_valid(line_details line, char* text, SymbolTable* symboltable, bool is_second_run);
bool is_label_valid_in_struct(line_details line, char* text);
bool is_label_valid_in_text(line_details line,char* text);
char* get_label(line_details line, SymbolTable* symboltable, bool is_second_run);
char* get_label_in_struct(char* text, char* label_name);
#endif /* LABEL_CHECK_H_ */

/*handles data,string,struct,extern, entry directives*/

#ifndef DIRECTIVES_HANDLER_H_
#define DIRECTIVES_HANDLER_H_
#include "utils.h"

typedef enum directive {
	_data, _string, _struct, _entry, _extern, _invalid
} directive;

directive str_to_directive(char* str);
directive find_directive_type(line_details line, char* begin);
void string_handler(line_details line, char* begin, int* DC, long* data_image);
void data_handler(struct line_details line, char* begin, int* DC, long* data_image);
void struct_handler(line_details line, char* begin, int* DC, long* data_image);


#endif /* DIRECTIVES_HANDLER_H_ */

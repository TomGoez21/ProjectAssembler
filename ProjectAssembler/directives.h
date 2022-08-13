/*handles data,string,struct,extern, entry directives*/

#ifndef DIRECTIVES_HANDLER_H_
#define DIRECTIVES_HANDLER_H_
#include "utils.h"

typedef enum directive {
	_data, _string, _struct, _entry, _extern, _invalid
} directive;

/*
 * Function:  str_to_directive
 * --------------------
 * convert string to the propper directive enum
 *
 * input: char* str
 * return: the matching directive enum
 */
directive str_to_directive(char* str);


/*
 * Function:  find_directive_type
 * --------------------
 * Checks type of directive
 *
 * input:	char* directive_string  : string to check
 *			char* begin  : point to the begin of the line
 *			line_details line  : struct of the line details
 * return: the matching directive enum
 */
directive find_directive_type(line_details line, char* begin, char* directive_string);


/*
 * Function:  find_directive_type
 * --------------------
 * Handles string directive
 *
 * input:	char* directive_string  : string to check
 *			char* begin  :  points to the first character
 *			line_details line  : struct of the line details
 */
void string_handler(line_details line, char* begin, long* DC, long** data_image_ptr);

/*
 * Function:  data_handler
 * --------------------
 * Handles string directive
 *
 * input:	char* directive_string  : string to check
 *			char* begin  :  points to the first character
 *			line_details line  : struct of the line details
 *			long *DC : data counter
 *			long** data_image_ptr
 *
 */
void data_handler(struct line_details line, char* begin, long* DC, long** data_image_ptr);

/*
 * Function:  struct_handler
 * --------------------
 * Handles string directive
 *
 * input:	char* directive_string  : string to check
 *			char* begin  :  points to the first character
 *			line_details line  : struct of the line details
 *			long *DC : data counter
 *			long** data_image_ptr
 *
 */
void struct_handler(line_details line, char* begin, long* DC, long** data_image_ptr);


bool is_directive(char* line);

#endif /* DIRECTIVES_HANDLER_H_ */

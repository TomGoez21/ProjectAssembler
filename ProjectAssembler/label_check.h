#ifndef LABEL_CHECK_H_
#define LABEL_CHECK_H_

#define MAX_LABEL_LENGTH 30

char* get_order(line_details line);

/*
 * Function:  get_order_num
 * --------------------
 * gets the decimal value of an order
 *
 * input:	char* order
 *
 * Returns: the number of the order from the order_list
 */
int* get_order_num(char* order);


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



/*
 * Function:  is_reserved_word
 * --------------------
 * check if text is one of the orders from order_list
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
 *
 * Returns: 1 if text equals to order from order_list
 */
bool is_label_valid(line_details line, char* text);



/*
 * Function:  is_label_valid_in_struct
 * --------------------
 * Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits
 *
 * input:	line_details line
 *			char* text - the curr line
 *
 * Returns: 1 if valid struct
 */
bool is_label_valid_in_struct(line_details line, char* text);



/*
 * Function:  is_label_valid
 * --------------------
 * Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits
 *
 * input:	line_details line
 *			char* text - the curr line
 *
 * Returns: 1 if valid struct
 */
bool is_label_valid_in_text(line_details line, char* text);




/*
 * Function:  get_label
 * --------------------
 * Check if the first char is alpha, length less than 30, all the others are alphanumeric, and that the label doesnt already exsits
 *
 * input:	line_details line
 *			char* text - the curr line
 *
 * Returns: 1 if valid struct
 */
char* get_label(line_details line);


char* get_label_in_struct(char* text, char* label_name);
#endif /* LABEL_CHECK_H_ */

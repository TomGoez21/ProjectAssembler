#ifndef SYMBOL_CONVERSION
#define SYMBOL_CONVERSION

/*
 * Function:  decimalToSymbolsBase
 *
 * INPUT: integer in a decimal base
 * RETURNS: pointer to char[3] of symobolBase value 
 */
char * decimalToSymbolsBase(int num);

/*
 * Function:  binToDecimal
 *
 * INPUT: an array of char[10] as a binary respresntaion
 * RETURNS: int of the number as decimal
 */
int binToDecimal(char *bin);

/*
 * Function:  decimalToBin
 *			parse and alocate memmory for binnary number
 * 
 * INPUT: pointer to char[3] of symobolBase value 
 * RETURNS: an array of char[10] as a binary respresntaion
 */
char * decimalToBin(int num);

/*
 * Function:  binToSymbolsBase
 *
 * INPUT: an array of char[10] as a binary respresntaion
 * RETURNS: pointer to char[3] of symobolBase value 
 */
char *binToSymbolsBase(char *bin);


#endif
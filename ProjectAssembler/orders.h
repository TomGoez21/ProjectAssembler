#ifndef ORDERS_H_
#define ORDERS_H_

#include "utils.h"

/* ARE types */
enum { ARE_ABSOLUTE = 0, ARE_REALOCATABLE = 2, ARE_EXTERNAL = 1 };

typedef struct order {
	char* order_name; 
	unsigned int opcode : 4; /* the opcode of the instruction */
	unsigned int src_addressing : 2; /* Legal addressing methods */
	unsigned int dst_addressing : 2; /* Legal addressing methods */
} order;



/** Commands opcode */
typedef enum order_opcodes {
	/*get 2 operands*/
	MOV, CMP, ADD, SUB, LEA,
	/*get one opernad*/
	NOT, CLR, INC, DEC, JMP, BNE, GET, PRN, JSR,
	/*do not get operands*/
	RTS, HLT,
	/*not an opernad*/
	NONE_OPCODE
} order_opcodes;

#endif
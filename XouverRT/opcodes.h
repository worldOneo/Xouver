#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define OP_ADD				(unsigned char) 0x8441
#define OP_SUB				(unsigned char) 0x9552
#define OP_MUL				(unsigned char) 0x2923
#define OP_DIV				(unsigned char) 0x3212
#define OP_JMP				(unsigned char) 0x9363
#define OP_IFEQ				(unsigned char) 0x8474
#define OP_IFGT				(unsigned char) 0x8475
#define OP_IFLT				(unsigned char) 0x8476
#define OP_IFGQ				(unsigned char) 0x8477
#define OP_IFLQ				(unsigned char) 0x8478
#define OP_NOT				(unsigned char) 0x8045
#define OP_JIF				(unsigned char) 0x7532
#define OP_CALL				(unsigned char) 0x9493	
#define OP_RET				(unsigned char) 0x2964
#define OP_YIELD			(unsigned char) 0x1044
#define OP_THROW			(unsigned char) 0x0132

#define OP_CLOAD			(unsigned char) 0x4243
#define OP_LLOAD			(unsigned char) 0x4249
#define OP_LSTORE			(unsigned char) 0x4204

#define OP_NEWO				(unsigned char) 0x3939
#define OP_OLOAD			(unsigned char) 0x3038
#define OP_OSTORE			(unsigned char) 0x3913
#define OP_INVO				(unsigned char) 0x9883

#define OP_POP				(unsigned char) 0x4858
#define OP_CLS				(unsigned char) 0x4859

#define OP_SYNC				(unsigned char) 0x1143
#define OP_ASYNC			(unsigned char) 0x1189

#define OP_LINE				(unsigned char) 0x4334

#ifdef _DEBUG
#define OP_OUT				(unsigned char) 0x5133
#endif

#endif
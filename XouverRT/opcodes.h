#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define OP_ADD				0xc8441
#define OP_SUB				0xc9552
#define OP_MUL				0xc2923
#define OP_DIV				0xc3212
#define OP_JMP				0xc9363
#define OP_IFEQ				0xc8474
#define OP_IFGT				0xc8475
#define OP_IFLT				0xc8476
#define OP_IFGQ				0xc8477
#define OP_IFLQ				0xc8478
#define OP_NOT				0xc9349
#define OP_JIF				0xc7532
#define OP_CALL				0xc9493	
#define OP_RET				0xc2964
#define OP_YIELD			0xc1044
#define OP_THROW			0xc0132

#define OP_CLOAD			0xc4243
#define OP_LLOAD			0xc4249
#define OP_LSTORE			0xc4204

#define OP_NEWO				0xc3939
#define OP_OLOAD			0xc3038
#define OP_OSTORE			0xc3913
#define OP_INVO				0xc3938

#define OP_POP				0xc4858
#define OP_CLS				0xc4859

#define OP_SYNC				0xc1143
#define OP_ASYNC			0xc1189

#define OP_LINE				0xc4334

#ifdef _DEBUG
#define OP_OUT				0xc5133
#endif

#endif
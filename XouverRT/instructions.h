#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define INST_ADD		(unsigned char) 0x241
#define INST_SUB		(unsigned char) 0x452
#define INST_MUL		(unsigned char) 0x423
#define INST_DIV		(unsigned char) 0x312
#define INST_INC		(unsigned char) 0x203
#define INST_DEC		(unsigned char) 0x344
#define INST_JMP		(unsigned char) 0x463
#define INST_IFEQ		(unsigned char) 0x874
#define INST_IFGT		(unsigned char) 0x875
#define INST_IFLT		(unsigned char) 0x876
#define INST_IFGQ		(unsigned char) 0x877
#define INST_IFLQ		(unsigned char) 0x878
#define INST_JIF		(unsigned char) 0x874
#define INST_CALL		(unsigned char) 0x893	
#define INST_NCALL		(unsigned char) 0x842
#define INST_RET		(unsigned char) 0x964
#define INST_YIELD		(unsigned char) 0x944
#define INST_THROW		(unsigned char) 0x332

#define INST_CLOAD		(unsigned char) 0x043
#define INST_OLOAD		(unsigned char) 0x834
#define INST_OSTORE		(unsigned char) 0x893
#define INST_LLOAD		(unsigned char) 0x393
#define INST_LSTORE		(unsigned char) 0x304

#define INST_LINE		(unsigned char) 0x034

#ifdef _DEBUG
#define INST_OUT		(unsigned char) 0x933
#endif

#endif
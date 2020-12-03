#ifndef NSCMD
#define NSCMD

// Non-compressed commands
#define K_ZERO '@'
#define K_SQRE ';'
#define K_DECR '['
#define K_INCR ']'
#define K_PUTD '.'
#define K_PUTC ','
#define K_GETD '"'
#define K_GETC '\''

#define K_LABL ':'
#define K_JUMP '|'
#define K_JMPZ '{'
#define K_JPNZ '}'
#define K_MOVL '<'
#define K_MOVR '>'
#define K_COMP '='
#define K_ERRO '!'

// 4bit commands
#define K_ZERO_M 0
#define K_SQRE_M 1
#define K_DECR_M 2
#define K_INCR_M 3
#define K_PUTD_M 4
#define K_PUTC_M 5
#define K_GETD_M 6
#define K_GETC_M 7
#define K_LABL_M 8
#define K_JUMP_M 9
#define K_JMPZ_M 10
#define K_JPNZ_M 11
#define K_MOVL_M 12
#define K_MOVR_M 13
#define K_COMP_M 14
#define K_ERRO_M 15

typedef unsigned char NS_CMD;

#define LO4BIT(cmd) ((cmd) & 0x0F)
#define HI4BIT(cmd) (((cmd) >> 4) & 0xF)

#endif

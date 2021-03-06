#ifndef CONSTS_H
#define CONSTS_H

#include <string.h>

#define MAX_STRLEN 500
#define MAX_LABEL 50
#define MAX_DMEM 4096

typedef struct {
	char* name;
	int address;
} label;

typedef struct {
	const char* name;
	char num;
} opcode;

typedef struct {
	char *name;
	char *rd;
	char *rs;
	char *rt;
	char *rm;
	char *imm1;
	char *imm2;
} cmd;

// Describing all given commands by name and number
const opcode commands[] = {
	{"add", 0},
	{"sub", 1},
	{"mac", 2},
	{"and", 3},
	{"or", 4},
	{"xor", 5},
	{"sll", 6},
	{"sra", 7},
	{"srl", 8},
	{"beq", 9},
	{"bne", 10},
	{"blt", 11},
	{"bgt", 12},
	{"ble", 13},
	{"bge", 14},
	{"jal", 15},
	{"lw", 16},
	{"sw", 17},
	{"reti", 18},
	{"in", 19},
	{"out", 20},
	{"halt", 21}
};

const opcode regs[] = {
	{"$zero", 0},
	{"$imm1", 1},
	{"$imm2", 2},
	{"$v0", 3},
	{"$a0", 4},
	{"$a1", 5},
	{"$a2", 6},
	{"$t0", 7},
	{"$t1", 8},
	{"$t2", 9},
	{"$s0", 10},
	{"$s1", 11},
	{"$s2", 12},
	{"$gp", 13},
	{"$sp", 14},
	{"$ra", 15}
};

#endif // COSNTS_H
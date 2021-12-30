#ifndef CONSTS_H
#define CONSTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRLEN	500
#define MAX_LABEL	50
#define MAX_MEM_LEN	4096
#define REG_LEN		4
#define IMM_LEN		12

// Registers
#define ZERO 	0
#define IMM1 	1
#define IMM2 	2
#define V0		3
#define A0		4
#define A1		5
#define A2		6
#define T0		7
#define T1		8
#define T2		9
#define S0		10
#define S1		11
#define S2		12
#define GP		13
#define SP		14
#define RA		15
#define REG_COUNT 16

//Hw Registers
#define IRQ0E 		0
#define IRQ1E 		1
#define IRQ2E 		2
#define IRQ0S		3
#define IRQ1S		4
#define IRQ2S		5
#define IRQ_H		6
#define IRQ_R		7
#define CLKS		8
#define LEDS		9
#define DISP		10
#define TIMER_E		11
#define TIMER_Q		12
#define TIMER_MAX	13
#define DISK_CMD	14
#define DISK_SEC	15
#define DISK_BUF	16
#define DISK_STAT	17
#define RSVD1		18
#define RSVD2		19
#define MON_ADDR	20
#define MON_DATA	21
#define MON_CMD		22
#define HW_COUNT	23

//argv[] indexing
#define IMEMIN 	1
#define DMEMIN	2
#define DISKIN	3
#define	IRQ2IN	4
#define DMEMOUT	5
#define REGOUT	6
#define TRACE	7
#define HWTRACE	8
#define CYCLES	9
#define LEDS_F	10
#define DISP_F	11
#define DISKOUT	12
#define MON_T	13
#define MON_YUV	14


typedef struct {
	char* name;
	int address;
} label;

typedef struct {
	const char* name;
	char num;
} opcode;

typedef struct {
	uint8_t name;
	uint8_t rd;
	uint8_t rs;
	uint8_t rt;
	uint8_t rm;
	int16_t imm1;
	int16_t imm2;
} cmd;

typedef struct {
	int max_addr;
	uint16_t mem[MAX_MEM_LEN];
} dmem;

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
#include "consts.h"

int hex2dec(char hex[]) {
	int len = strlen(hex), base = 1, dec = 0;
	for (int i = len - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9')
			dec += (hex[i] - 48) * base;
        else if (hex[i] >= 'A' && hex[i] <= 'F')
			dec += (hex[i] - 55) * base;
		else if (hex[i] >= 'a' && hex[i] <= 'f')
			dec += (hex[i] - 'a' + 10) * base;
        base *= 16;
    }
    return dec;
}
// //
// uint16_t get_val(uint64_t line) {
// 	uint16_t mask = 0;

// }

void write_trace(uint64_t line, uint16_t PC, const char fname[], int regs[]) {
	char str[MAX_STRLEN], temp_str[MAX_STRLEN];
	FILE *fp;
	fp = fopen(fname, "a");
	sprintf(str, "%03X %012llX ", PC, line);
	for (int i = 0; i < REG_COUNT; i++) {
		sprintf(temp_str, "%08x ", TO_SIGNED(regs[i]));
		strcat(str, temp_str);
	}
	fprintf(fp, "%s\n", str);
	fclose(fp);
}

void write_clk(const char fname[], int clks) {
	FILE *fp;
	fp = fopen(fname, "w+");
	fprintf(fp, "%d\n", clks);
	fclose(fp);
}


void sim(char const *argv[]) {
	int regs[REG_COUNT] = {0};
	uint32_t hw_regs[HW_COUNT] = {0};
	uint16_t PC = 0;
	FILE *imemin, *clk;
	cmd lines[MAX_MEM_LEN] = {0};


	imemin = fopen(argv[IMEMIN], "r");
	while(fscanf(imenin, "%02X%01X%01X%01X%01X%03X%03X", line.name, line.rd, line.rs, line.rm, line.imm1, line.imm2) != EOF) {
		cmd_regs(lines, hw_regs, regs);
	}

	write_clk(argv[CYCLES], hw_regs[CLKS]);
	
	fclose(imemin);
}

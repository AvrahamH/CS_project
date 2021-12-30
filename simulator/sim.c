#include "consts.h"


#define TO_SIGNED(num) ((num & 0xFFF) | ((num & 0x800) ? ~0xFFF : 0))

void cmd_regs(cmd lines[MAX_MEM_LEN], uint32_t hw_regs[HW_COUNT],int regs[REG_COUNT],const * argv[]){
	int pc = 0; cmd_line = 0;
	// now we are going through the command lines and change the registers according to the command 
	while(line[cmd_Line].name != 21){
		write_trace(line[cmd_line],pc,argv[TRACE],regs); // every command we wanr to update the trace
		// chacking if there is a illegal change to a ZERO or IMM registers  
		if(lines[cmd_line].rd  == ZERO | lines[cmd_line].rd  == IMM1 | lines[cmd_line].rd  == IMM2  ){
			pc ++;
			hw_regs[CLKS]++;
			cmd_line ++;
			continue;
			}
		// else we change the imm registers if we use them in the current command and  implement the command according to the operation 
		else{
			if((lines[cmd_line].rs == IMM1 | lines[cmd_line].rs == IMM2)| (lines[cmd_line].rt == IMM1 | lines[cmd_line].rt == IMM2) | (lines[cmd_line].rm == IMM1 | lines[cmd_line].rm == IMM2)){
				if(lines[cmd_line].rs == IMM1 | lines[cmd_line].rs == IMM2){
					if(lines[cmd_line].rs == IMM1)
						lines[cmd_line].rs == lines[cmd_line].imm1;
					else
						lines[cmd_line].rs == lines[cmd_line].imm2;	
				}	
				if(lines[cmd_line].rt == IMM1 | lines[cmd_line].rt == IMM2){
					if(lines[cmd_line].rt == IMM1)
						lines[cmd_line].rt == lines[cmd_line].imm1;
					else
						lines[cmd_line].rt == lines[cmd_line].imm2;			
				}
				if(lines[cmd_line].rm == IMM1 | lines[cmd_line].rm == IMM2){
					if(lines[cmd_line].rm == IMM1)
						lines[cmd_line].rm == lines[cmd_line].imm1;
					else
						lines[cmd_line].rm == lines[cmd_line].imm2;			
				}
			}
			switch(lines[cmd_line].name){
				case 0 :
					regs[lines[cmd_line].rd] = regs[cmd_line.rs] + regs[cmd_line.rt] + regs[cmd_line.rm];
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 1 :
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] - regs[lines[cmd_line].rt] - regs[lines[cmd_line].rm];
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 2:
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] * regs[lines[cmd_line].rt] - regs[lines[cmd_line].rm];
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 3 :
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] & regs[lines[cmd_line].rt] & regs[lines[cmd_line].rm];
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 4:
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] | regs[lines[cmd_line].rt] | regs[lines[cmd_line].rm];
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 5 :
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] ^ regs[lines[cmd_line].rt] ^ regs[lines[cmd_line].rm];
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 6:
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] << regs[lines[cmd_line].rt] ;
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 7 :
					//	NEED TO DO ARITH SHIFT WITH SIGN EXTANSION 
					break;
				case 8:
					regs[lines[cmd_line].rd] = regs[lines[cmd_line].rs] >> regs[lines[cmd_line].rt] ;
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 9 :
					if(regs[lines[cmd_line].rs] == regs[lines[cmd_line].rt]){
						pc = regs[lines[cmd_line].rm];
						hw_regs[CLKS]++;
						cmd_line = regs[lines[cmd_line].rm];
						break;
					}
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 10:
					if(regs[lines[cmd_line].rs] != regs[lines[cmd_line].rt]){
						pc = regs[lines[cmd_line].rm];
						hw_regs[CLKS]++;
						cmd_line = regs[lines[cmd_line].rm];
						break;
					}
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 11 :
					if(regs[lines[cmd_line].rs] < regs[lines[cmd_line].rt]){
						pc = regs[lines[cmd_line].rm];
						hw_regs[CLKS]++;
						cmd_line = regs[lines[cmd_line].rm];
						break;
					}
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 12:
					if(regs[lines[cmd_line].rs] > regs[lines[cmd_line].rt]){
						pc = regs[lines[cmd_line].rm];
						hw_regs[CLKS]++;
						cmd_line = regs[lines[cmd_line].rm];
						break;
					}
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 13 :
					if(regs[lines[cmd_line].rs] <= regs[lines[cmd_line].rt]){
						pc = regs[lines[cmd_line].rm];
						hw_regs[CLKS]++;
						cmd_line = regs[lines[cmd_line].rm];
						break;
					}
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 14:
					if(regs[lines[cmd_line].rs] >= regs[lines[cmd_line].rt]){
						pc = regs[lines[cmd_line].rm];
						hw_regs[CLKS]++;
						cmd_line = regs[lines[cmd_line].rm];
						break;
					}
					pc ++;
					hw_regs[CLKS]++;
					cmd_line ++;
					break;
				case 15 :
					regs[lines[cmd_line].rd] = pc + 1;
					pc = regs[lines[cmd_line].rm];
					hw_regs[CLKS]++;
					cmd_line = regs[lines[cmd_line].rm];
					break;
				case 16:
				break;
				case 17 :
				break;
				case 18:
				break;
				case 19 :
				break;
				case 20:
				break;
				}
			}
		}
	}


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

void write_trace(uint64_t line, uint16_t PC, const char fname[], int regs[]) {
	char str[MAX_STRLEN], temp_str[MAX_STRLEN];
	FILE *fp;
	fp = fopen(fname, "a");
	sprintf(str, "%03X %012llX ", PC, line);
	for (int i = 0; i < REG_COUNT; i++) {
		if (i == IMM1 || i == IMM2)
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
	int line = 0;
	uint32_t hw_regs[HW_COUNT] = {0}, regs[REG_COUNT] = {0}, dmem[MAX_MEM_LEN] = {0};
	uint16_t PC = 0;
	FILE *imemin, *dmemin, *clk;
	cmd lines[MAX_MEM_LEN] = {0};

	imemin = fopen(argv[IMEMIN], "r");
	while(fscanf(imemin, "%02X%01X%01X%01X%01X%03X%03X", &lines[line].name, &lines[line].rd, &lines[line].rs, &lines[line].rt,&lines[line].rm, &lines[line].imm1, &lines[line].imm2) != EOF) {
		// cmd_regs(lines, hw_regs, regs);
		line++;
	}

	write_clk(argv[CYCLES], hw_regs[CLKS]);
	
	fclose(imemin);
}

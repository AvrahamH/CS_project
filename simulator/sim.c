#include "consts.h"

#define TO_SIGNED(x) ((x >> 11) == 0 ? x : -1 ^ 0xFFF | x)
#define SIGN2INT(i) ((int)(IS_IMM(i) ? TO_SIGNED(regs[i]) & 0xFFFFFFFF : regs[i]))


void write_trace(cmd line, uint16_t PC, const char fname[], uint32_t regs[]) {
	char str[MAX_STRLEN], temp_str[MAX_STRLEN] = "00000000 ";
	FILE *fp;
	fp = fopen(fname, "a");
	sprintf(str, "%03X %02X%01X%01X%01X%01X%03X%03X", PC, line.name, line.rd, line.rs, line.rt, line.rm, line.imm1 & 0xFFF, line.imm2 & 0xFFF);
	for (int i = 0; i < REG_COUNT; i++) {
		sprintf(temp_str, " %08x", IS_IMM(i) ? TO_SIGNED(regs[i]) & 0xFFFFFFFF : regs[i]);
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

void write_regout(const char fname[], uint32_t regs[REG_COUNT - 3]) {
	FILE *fp;
	fp = fopen(fname, "w+");
	for (int i = V0; i < REG_COUNT; i++)
		fprintf(fp, "%08X\n", regs[i]);
	fclose(fp);
}

void write_hwtrace(const char fname[], uint8_t reg_num, uint32_t hw_regs[HW_COUNT], uint8_t op) {
	FILE *fp;
	fp = fopen(fname, "a");
	if (op == IN)
		fprintf(fp, "%03d READ %s %08X\n", hw_regs[CLKS], hw_regs_names[reg_num], hw_regs[reg_num]);
	else
		fprintf(fp, "%03d WRITE %s %08X\n", hw_regs[CLKS], hw_regs_names[reg_num], hw_regs[reg_num]);
	fclose(fp);
}

void write_leds(const char fname[], uint32_t hw_regs[HW_COUNT]) {
	FILE *fp;
	fp = fopen(fname, "a");
	fprintf(fp, "%03d %08X", hw_regs[CLKS], hw_regs[LEDS]);
	fclose(fp);
}

void write_disp(const char fname[], uint32_t hw_regs[HW_COUNT]) {
	FILE *fp;
	fp = fopen(fname, "a");
	fprintf(fp, "%03d %08X", hw_regs[CLKS], hw_regs[DISP]);
	fclose(fp);
}

void write_monitor(const char *txt, const char *yuv, uint8_t monitor[MON_SIZE]) {
	FILE *fp1, *fp2;
	fp1 = fopen(txt, "w+");
	fp2 = fopen(yuv, "wb");
	for (int i = 0; i < MON_SIZE; i++) {
		fprintf(fp1, "%02X\n", monitor[i]);
		fwrite(monitor + i, 1, 1, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void write_mem(mem *memory, int type, char const *argv[]) {
	FILE *fp;
	fp = fopen(argv[type], "w+");
	for (int i = 0; i < memory->max_addr; i++)
		fprintf(fp, "%08X\n", memory->mem[i]);
	fclose(fp);
}

void write_disk(disk *memory, int type, char const *argv[]) {
	FILE *fp;
	fp = fopen(argv[type], "w+");
	for (int i = 0; i < memory->max_addr; i++)
		fprintf(fp, "%08X\n", memory->mem[i]);
	fclose(fp);
}
/**
 * irq_flag: equals 0 by default, equals 1 if we're in an interrupt
 * disk_flag: to indicate if disk cmd, buffer and sector has been received
 */
void cmd_regs(cmd lines[MAX_MEM_LEN], mem *dmemout, disk *diskout, uint32_t hw_regs[HW_COUNT], uint32_t regs[REG_COUNT], char const *argv[]) {
	int pc = 0, msb_1 = 1 << 31, irq = 0, irq2arr[MAX_MEM_LEN] = {0}, irq2clk = 0, irq_flag = 0, disk_timer = 0, disk_flag = 0;
	uint8_t monitor[MON_SIZE] = {0}, temp_reg_val = 0;
	cmd *temp_line;
	FILE *irq2in;

	irq2in = fopen(argv[IRQ2IN], "r");
	for (int i = 0; fscanf(irq2in, "%d\n", &irq2clk) != EOF; i++)
		irq2arr[irq2clk] = 1;
	fclose(irq2in);

	// now we are going through the command lines and change the registers according to the command 
	while((lines[pc].name != HALT) && (pc < 4095)) {
		if (irq2arr[hw_regs[CLKS]]) //handling 
			hw_regs[IRQ2S] = 1;

		if (hw_regs[TIMER_E]) {	//handling timer
			hw_regs[TIMER_C]++;
			if (hw_regs[TIMER_MAX] == hw_regs[TIMER_C]) {
				hw_regs[IRQ0S] = 1;
				hw_regs[TIMER_C] = 0;
			}
		}

		irq = (hw_regs[IRQ0E] && hw_regs[IRQ0S]) || (hw_regs[IRQ1E] && hw_regs[IRQ1S]) || (hw_regs[IRQ1E] && hw_regs[IRQ2S]);

		if (irq && !irq_flag) {
			irq_flag = 1;
			pc = hw_regs[IRQ_H];
		}

		if (hw_regs[DISK_STAT]) {	//raise clock or finish interrupt
			if (disk_timer == 1024) {
				disk_timer = 0;
				disk_flag = 0;
				hw_regs[DISK_CMD] = 0;
				hw_regs[DISK_STAT] = 0;
				hw_regs[IRQ1S] = 1;
			} else 
				disk_timer++;
		}

		regs[IMM1] = regs[IMM2] = 0;
		temp_line = lines + pc;

		if (IS_IMM(temp_line->rs))
				regs[temp_line->rs] = (temp_line->rs == IMM1 ? temp_line->imm1 : temp_line->imm2) & 0xFFF;

		if (IS_IMM(temp_line->rt))
				regs[temp_line->rt] = (temp_line->rt == IMM1 ? temp_line->imm1 : temp_line->imm2) & 0xFFF;

		if (IS_IMM(temp_line->rm))
				regs[temp_line->rm] = (temp_line->rm == IMM1 ? temp_line->imm1 : temp_line->imm2) & 0xFFF;

		temp_reg_val = regs[temp_line->rs] + regs[temp_line->rt];

		write_trace(lines[pc], pc, argv[TRACE], regs); // every command we want to update the trace

		// checking if there is a illegal change to a ZERO or IMM registers  
		if (IS_WRITEABLE(temp_line->rd) && ((temp_line->name >= ADD && temp_line->name <= SRL) || temp_line->name == JAL || temp_line->name == LW || temp_line->name == IN)) {
			pc++;
			hw_regs[CLKS]++;
			continue;
		}
		// else we change the imm registers if we use them in the current command and execute the command according to the operation 
		else {
			switch (temp_line->name) {
				case ADD:
					regs[temp_line->rd] = SIGN2INT(temp_line->rs) + SIGN2INT(temp_line->rt) + SIGN2INT(temp_line->rm);
					break;
				case SUB:
					regs[temp_line->rd] = SIGN2INT(temp_line->rs) - SIGN2INT(temp_line->rt) - SIGN2INT(temp_line->rm);;
					break;
				case MAC:
					regs[temp_line->rd] = SIGN2INT(temp_line->rs) * SIGN2INT(temp_line->rt) - SIGN2INT(temp_line->rm);;
					break;
				case AND:
					regs[temp_line->rd] = regs[temp_line->rs] & regs[temp_line->rt] & regs[temp_line->rm];
					break;
				case OR:
					regs[temp_line->rd] = regs[temp_line->rs] | regs[temp_line->rt] | regs[temp_line->rm];
					break;
				case XOR:
					regs[temp_line->rd] = regs[temp_line->rs] ^ regs[temp_line->rt] ^ regs[temp_line->rm];
					break;
				case SLL:
					regs[temp_line->rd] = regs[temp_line->rs] << regs[temp_line->rt];
					break;
				case SRA:
					regs[temp_line->rd] = regs[temp_line->rs] >> regs[temp_line->rt];
					break;
				case SRL:
					if(regs[temp_line->rs] & msb_1){
						regs[temp_line->rd] = (int)((unsigned int)regs[temp_line->rs] >> regs[temp_line->rt]) ; 
					}
					else
						regs[temp_line->rd] = regs[temp_line->rs] >> regs[temp_line->rt] ;
					pc++;
					hw_regs[CLKS]++;
					break;	
				case BEQ:
					if(regs[temp_line->rs] == regs[temp_line->rt]){
						pc = regs[temp_line->rm] & 0xFFF;
						hw_regs[CLKS]++;
						break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
				case BNE:
					if (regs[temp_line->rs] != regs[temp_line->rt]) {
						pc = regs[temp_line->rm] & 0xFFF;
						hw_regs[CLKS]++;
						break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
				case BLT:
					if (regs[temp_line->rs] < regs[temp_line->rt]) {
						pc = regs[temp_line->rm] & 0xFFF;
						hw_regs[CLKS]++;
						break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
				case BGT:
					if (regs[temp_line->rs] > regs[temp_line->rt]) {
						pc = regs[temp_line->rm] & 0xFFF;
						hw_regs[CLKS]++;
						break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
				case BLE:
					if(regs[temp_line->rs] <= regs[temp_line->rt]){
						pc = regs[temp_line->rm] & 0xFFF;
						hw_regs[CLKS]++;
						break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
				case BGE:
					if(regs[temp_line->rs] >= regs[temp_line->rt]){
						pc = regs[temp_line->rm] & 0xFFF;
						hw_regs[CLKS]++;
						break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
				case JAL:
					regs[temp_line->rd] = pc + 1;
					pc = regs[temp_line->rm] & 0xFFF;
					hw_regs[CLKS]++;
					break;
				case LW:
					regs[temp_line->rd] = dmemout->mem[temp_reg_val] + regs[temp_line->rm];
					pc++;
					hw_regs[CLKS]++;
					break;
				case SW:
					dmemout->mem[temp_reg_val] = regs[temp_line->rm] + regs[temp_line->rd];
					dmemout->max_addr = dmemout->max_addr < temp_reg_val ? temp_reg_val : dmemout->max_addr;
					pc++;
					hw_regs[CLKS]++;
					break;
				case RETI:
					irq_flag = 0;
					pc = hw_regs[IRQ_R];
					break;
				case IN:
					switch (temp_reg_val) {
						case MON_CMD:
							temp_line->rd = 0;
							break;
						default:
							temp_line->rd = hw_regs[temp_reg_val];
							break;
					}
					break;
				case OUT:
					switch (temp_reg_val) {
						case IRQ0E:
						case IRQ1E:
						case IRQ2E:
						case TIMER_E:
							hw_regs[temp_reg_val] = 1;
							break;
						case DISP:
							hw_regs[DISP] = temp_line->rm;
							write_disp(argv[DISP_F], hw_regs);
							break;
						case LEDS:
							hw_regs[LEDS] = temp_line->rm;
							write_leds(argv[LEDS], hw_regs);
							break;
						case DISK_CMD:
							if (hw_regs[DISK_STAT])
								break;
							else {
								if (disk_flag == 2) {
									hw_regs[DISK_STAT] = 1;
									hw_regs[temp_reg_val] = temp_line->rm;
								}
							}
							break;
						case DISK_SEC:
						case DISK_BUF:
							if (hw_regs[DISK_STAT]) {
								hw_regs[temp_reg_val] = temp_line->rm;
								disk_flag++;
							}
							break;
						default:
							hw_regs[temp_reg_val] = temp_line->rm;
							break;
					}
					pc++;
					hw_regs[CLKS]++;
					break;
			}
			if (temp_line->name >= 0 && temp_line->name < 8) {
				pc++;
				hw_regs[CLKS]++;
			}

			if (temp_line->name == IN || temp_line->name == OUT)
				write_hwtrace(argv[HWTRACE], temp_line->rs + temp_line->rt, hw_regs, temp_line->name);

			if (hw_regs[DISK_CMD] && disk_timer == 1024) {
				for (int i = 0; i < SEC_SIZE; i++) {
					if (hw_regs[DISK_CMD] == 1) {	//read
						*((uint32_t *)hw_regs[DISK_BUF] + i) = dmemout->mem[128 * hw_regs[DISK_SEC] + i];
					}
					else { 	//write
						dmemout->mem[128 * hw_regs[DISK_SEC] + i] = *((uint32_t *)hw_regs[DISK_BUF] + i);
					}
				}
			}

			if (hw_regs[MON_CMD]) {
				monitor[hw_regs[MON_ADDR]] = hw_regs[MON_DATA];
				hw_regs[MON_CMD] = 0;
			}

		}
	}
	write_regout(argv[REGOUT], regs + 3);
	write_monitor(argv[MON_T], argv[MON_YUV], monitor);
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

void sim(char const *argv[]) {
	int line = 0;
	uint32_t hw_regs[HW_COUNT] = {0}, regs[REG_COUNT] = {0};
	uint16_t PC = 0;
	FILE *imemin, *dmemin, *diskin;
	cmd lines[MAX_MEM_LEN] = {0};

	FILE *fp;
	fp = fopen(argv[TRACE], "w");
	fclose(fp);
	fp = fopen(argv[LEDS], "w");
	fclose(fp);
	fp = fopen(argv[DISP], "w");
	fclose(fp);
	fp = fopen(argv[HWTRACE], "w");
	fclose(fp);

	mem *dmemout = (mem *)calloc(1, sizeof(mem));
	dmemin = fopen(argv[DMEMIN], "r");
	for (line = 0; fscanf(dmemin, "%08X", &(dmemout->mem[line])) != EOF; line++);
	fclose(dmemin);
	dmemout->max_addr = line - 1;

	imemin = fopen(argv[IMEMIN], "r");
	for (line = 0; fscanf(imemin, "%02X%01X%01X%01X%01X%03X%03X", &lines[line].name, &lines[line].rd, &lines[line].rs, &lines[line].rt, &lines[line].rm, &lines[line].imm1, &lines[line].imm2) != EOF; line++);
	fclose(imemin);
	
	disk *diskout = (disk *)calloc(1, sizeof(disk));
	diskin = fopen(argv[DISKIN], "r");
	for (line = 0; fscanf(diskin, "%08X", &(diskout->mem[line])) != EOF; line++);
	fclose(diskin);
	diskout->max_addr = line > 0 ? line - 1 : 0;

	cmd_regs(lines, dmemout, diskout, hw_regs, regs, argv);

	write_clk(argv[CYCLES], hw_regs[CLKS]);
	write_mem(dmemout, DMEMOUT, argv);
	write_disk(diskout, DISKOUT, argv);

	free(dmemout);
	free(diskout);
}

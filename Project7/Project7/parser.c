#include <stdio.h>
#include <stdlib.h>
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

uint64_t str2num(char *name, int size, const opcode *arr) {
	for(int i = 0; i < size; i++) {
		if(!strcmp(arr[i].name, name))
			return (uint64_t)arr[i].num;
	}
	return -1;
}

uint64_t label2addr(char *name, int size, label *arr) {
	for(int i = 0; i < size; i++) {
		if(!strcmp(arr[i].name, name))
			return (uint64_t)arr[i].address;
	}
	return -1;
}

void parse_cmd(char *line, label *label_arr, int label_count, char *imemin, char *dmemin) {
	FILE *fp, *dmem;
	int count = 0, addr = 0, words[MAX_DMEM] = { 0 }, max_addr = 0, i = 0;
	uint16_t fline[7] = { 0 };
	char *token, temp_line[MAX_STRLEN];

	strcpy(temp_line, line);
	fp = fopen(imemin, "a");
	
	token = strtok(temp_line, "#");
	token = strtok(token, " \r\n\t");

	if (!strcmp(token, ".word")) {
		token = strtok(NULL, " ,\n\t\r");
		if (*token == '0' && (*(token + 1) == 'x' || *(token + 1) == 'X'))
			addr = hex2dec(token + 2);
		else 
			addr = atoi(token);

		token = strtok(NULL, " ,\n\t\r");
		if (*token == '0' && (*(token + 1) == 'x' || *(token + 1) == 'X'))
			words[addr] = hex2dec(token + 2);
		else 
			words[addr] = atoi(token);

		max_addr = (addr > max_addr) ? addr : max_addr;
	} else {
		while (token != NULL) {
			switch (count) {
				case 0:
					fline[0] = str2num(token, 22, commands);
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					fline[count] = str2num(token, 16, regs);
					break;
				case 5:
				case 6:
					addr = label2addr(token, label_count, label_arr);
					if (addr != -1)
						fline[count] = addr;
					else {
						if (*token == '0' && (*(token + 1) == 'x' || *(token + 1) == 'X'))
							fline[count] = hex2dec(token + 2);
						else
							fline[count] = atoi(token);
					}
					break;

			}
			token = strtok(NULL, " ,\n\t\r");
			count++;
		}
		fprintf(fp, "%02X%01X%01X%01X%01X%03X%03X\n", fline[0], fline[1], fline[2], fline[3], fline[4], fline[5] & 0XFFF, fline[6] & 0XFFF);
	}

	dmem = fopen(dmemin, "w+");

	for (i = 0; i < max_addr + 1; i++)
		fprintf(dmem, "%08X\n", words[i]);

	fclose(dmem);

	fclose(fp);
}

// TODO: support exit

void parse_asm(char *program, char *imemin, char *dmemin) {
	FILE *fp;
	char line[MAX_STRLEN], *str, temp_line[MAX_STRLEN];
	int line_count = 0, label_count = 0;

	label *label_arr;
	label_arr = (label*)malloc(sizeof(label));
	if (label_arr == NULL) {
		printf("allocation failed!\n");
	}

	fp = fopen(program, "r");
	while (fgets(line, MAX_STRLEN, fp)) {
		strcpy(temp_line, line);
		str = strtok(temp_line, ":");
		if (strcmp(line, str)) {	//if it's a label
			label_arr[label_count].name = (char*)malloc(sizeof(char) * MAX_LABEL);
			strcpy(label_arr[label_count].name, str);
			label_arr[label_count].address = (line_count - label_count);
			label_count++;
			label_arr = (label*)realloc(label_arr, sizeof(label) * (label_count + 1));		
		}
		line_count++;
	}

	rewind(fp);

	while (fgets(line, MAX_STRLEN, fp)) {
		strcpy(temp_line, line);
		str = strtok(temp_line, ":");
		if (!strcmp(line, str))	//if it's a command
			parse_cmd(line, label_arr, label_count, imemin, dmemin);
		else {					//if it's a label
			continue;
		}
	}

	for (int i = label_count - 1; i >= 0; i--)
		free(label_arr[i].name);
	free(label_arr);
	fclose(fp);
}
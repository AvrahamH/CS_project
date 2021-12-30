#include <stdio.h>
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

void parse_cmd(char *line) {

}

void parse_asm(char *fname) {
	FILE *fp;
	char line[MAX_STRLEN], str[MAX_LABEL];
	label x = {0, 0};
	int line_count = 0;

	fp = fopen(fname, "r");
	while (fgets(line, MAX_STRLEN, fp)) {

		str = strtok(line, ":");
		if (strcmp(line, str))	//if it's a command
			continue;
		else {					//if it's a label
			strcpy(x.name, str);
			x.address = line_count;
		}
		line_count++;
	}

	rewind(fp);
	line_count = 0;

	while (fgets(line, MAX_STRLEN, fp)) {
		str = strtok(line, ":");
		if (strcmp(line, str))	//if it's a command
			continue;
		else {					//if it's a label
			
		}
	}	

	fclose(fp);
}
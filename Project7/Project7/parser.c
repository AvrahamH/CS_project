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

void parse_cmd(char *line) {

}

// TODO: support exit

void parse_asm(char *fname) {
	FILE *fp;
	char line[MAX_STRLEN], *str, temp_line[MAX_STRLEN];
	int line_count = 0, label_count = 1;

	label *label_arr;
	label_arr = (label*)malloc(sizeof(label));
	if (label_arr == NULL) {
		printf("allocation failed!\n");
	}

	fp = fopen(fname, "r");
	while (fgets(line, MAX_STRLEN, fp)) {
		strcpy(temp_line, line);
		str = strtok(temp_line, ":");
		if (strcmp(line, str)) {	//if it's a label
			strcpy(label_arr[label_count - 1].name, str);
			label_arr[label_count - 1].address = line_count;
			label_count++;
			label_arr = (label*)realloc(label_arr, sizeof(label) * label_count);
			// memcpy((label_arr + label_count - 1), &lab, sizeof(label));				
		}
		line_count++;
	}

	rewind(fp);
	line_count = 0;

	while (fgets(line, MAX_STRLEN, fp)) {
		str = strtok(line, ":");
		if (!strcmp(line, str))	//if it's a command
			continue;
		else {					//if it's a label
			
		}
	}	

	fclose(fp);
}
#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[]) {
	// parse_asm(argv[1]);
	char str[] = "0xFFFF";
	int i = hex2dec(&str[2]);
	printf("%x\n", i);
	return 0;
}

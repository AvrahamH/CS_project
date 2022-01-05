#include <stdio.h>
#include "sim.h"

//sim.exe imemin.txt dmemin.txt diskin.txt irq2in.txt dmemout.txt regout.txt trace.txt hwregtrace.txt cycles.txt leds.txt display7seg.txt diskout.txt monitor.txt monitor.yuv
int main(int argc, char const *argv[])
{
    sim(argv);
    return 0;
}
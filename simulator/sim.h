#ifndef SIM_H
#define SIM_H

#define IS_WRITEABLE (reg) (reg == ZERO || reg == IMM1 || reg == IMM2)

void sim(char const *argv[]);

#endif /* SIM_H */
#ifndef SIM_H
#define SIM_H

#define TO_SIGNED(num, bytes) ((num & (((1 << bytes) - 1))) | ((num & (1 << (bytes - 1))) ? ~((1 << bytes) - 1) : 0))

void sim(char const *argv[]);

#endif /* SIM_H */
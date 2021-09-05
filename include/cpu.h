#ifndef EMULATOR101_CPU_H
#define EMULATOR101_CPU_H

#define WORD(x, y) ((x<<8) | y)

void init(long fsize, FILE *f, long start_addr);

void emulate_step();

#endif //EMULATOR101_CPU_H

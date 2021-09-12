#ifndef EMULATOR101_CPU_H
#define EMULATOR101_CPU_H

#define WORD(x, y) ((x<<8) | y)

void load_test_code();

void allocate_memory(long memsize);

void load(long fsize, FILE *f);

void free();

void init(long start_addr);

void emulate_step();

#endif //EMULATOR101_CPU_H

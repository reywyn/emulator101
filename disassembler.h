#ifndef EMULATOR101_DISASSEMBLER_H
#define EMULATOR101_DISASSEMBLER_H

#define READ_2_BYTE printf("%02x%02x", code[2], code[1]); opbytes=3
#define READ_1_BYTE printf("%02x", code[1]); opbytes=2

int disassemble(unsigned char *codebuffer, int _pc);

#include "disassembler.cpp"

#endif //EMULATOR101_DISASSEMBLER_H

#include <cstdio>
#include <cstdlib>

#include "disassembler.h"
#include "cpu.h"

int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "rb");
    if (f == nullptr) {
        printf("error: Couldn't open %s\n", argv[1]);
        exit(1);
    }

    //Get the file size and read it into a memory buffer
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    auto *buffer = static_cast<unsigned char *>(malloc(fsize));

    fread(buffer, fsize, 1, f);
    fclose(f);

    int pc = 0;

    while (pc < fsize) {
        pc += disassemble(buffer, pc);
    }
    return 0;
}

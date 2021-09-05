#include <cstdio>
#include <cstdlib>
#include "cpu.h"

long load_game(char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (f == nullptr) {
        printf("error: Couldn't open %s\n", filepath);
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    init(fsize, f);
    fclose(f);

    return fsize;
}

int main(int argc, char **argv) {


    long fsize = load_game(argv[1]);

    while (true) {
        emulate_step();
    }




    /* FILE *f = fopen(argv[1], "rb");
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
     return 0;*/
}

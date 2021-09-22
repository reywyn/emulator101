#include <cstdio>
#include <cstdlib>
#include <string>
#include "cpu.h"


long init_test() {
    init(0x100);
    allocate_memory(0x10000);
    load_test_code();
}


long load_rom(char *filepath, bool should_alloc) {
    FILE *f = fopen(filepath, "rb");
    if (f == nullptr) {
        printf("error: Couldn't open %s\n", filepath);
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    if (should_alloc)
        allocate_memory(fsize);

    load(fsize, f);
    fclose(f);

    return fsize;
}


void run() {
    while (true) {
        emulate_step();
    }
}

int main(int argc, char **argv) {

    if (strcmp(argv[1], "test") == 0) {
        init_test();
        load_rom((char *) "/Users/emre/Projects/emre/emulator101/roms/test/TST8080.COM", false);
        run();
        free();

        init_test();
        load_rom((char *) "test/CPUTEST.COM", false);
        free();

        init_test();
        load_rom((char *) "test/8080PRE.COM", false);
        free();

        init_test();
        load_rom((char *) "test/8080EXM.COM", false);
        free();

        exit(1);
    }

    init(0);
    load_rom(argv[1], true);
    run();
    free();
}

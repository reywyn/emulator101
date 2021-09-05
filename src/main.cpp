#include <cstdio>
#include <cstdlib>
#include <string>
#include "cpu.h"

long load_game(char *filepath, long start_addr) {
    FILE *f = fopen(filepath, "rb");
    if (f == nullptr) {
        printf("error: Couldn't open %s\n", filepath);
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    init(fsize, f, start_addr);
    fclose(f);

    return fsize;
}

int main(int argc, char **argv) {
    if (argc == 2)
        long fsize = load_game(argv[1], 0);
    else if (argc == 3)
        long fsize = load_game(argv[1], std::stol(argv[2], 0, 16));
    else
        exit(1);

    while (true) {
        emulate_step();
    }
}

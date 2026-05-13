#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OUTPUT "code.ips"

void writeInt(long n, size_t len, FILE *fptr) {
    while (len > 0) {
        unsigned char byte = (n >> (len - 1)) % 0x100;
        fwrite(&byte, 1, 1, fptr);
        len--;
    }
}

void nuke(long offset) {
    FILE *fptr = fopen(OUTPUT, "wb");

    fwrite("PATCH", 1, 5, fptr);

    writeInt(offset, 3, fptr);
    writeInt(0x4, 2, fptr);

    // bkpt instruction
    writeInt(0x700020e1, 4, fptr);

    fwrite("EOF", 1, 3, fptr);

    fclose(fptr);
}

int main(int argc, char **argv) {
    printf("(remember: ghidra addresses are 0x00100000 too big!)\n");

    long offset = strtol(argv[1], NULL, 16);

    nuke(offset);
    printf("made a nuke for code.bin offset 0x%lx\n", offset);

    return 0;
}

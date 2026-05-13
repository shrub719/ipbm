#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OUTPUT "code.ips"

void writeBytes(long bytes, size_t len, FILE *fptr) {
    while (len > 0) {
        unsigned char byte = (bytes >> (len - 1) * 8) % 0x100;
        fwrite(&byte, 1, 1, fptr);
        len--;
    }
}

void nuke(long offset) {
    FILE *fptr = fopen(OUTPUT, "wb");

    fwrite("PATCH", 1, 5, fptr);

    writeBytes(offset, 3, fptr);
    writeBytes(0x4, 2, fptr);

    // I HAVE YET to discover what instruction/s make the emulator
    //   for sure crash or stop. none of these work
    //
    // bkpt instruction
    // writeBytes(0x700020e1, 4, fptr);
    //
    // garbage
    writeBytes(0xdeadbeef, 4, fptr);
    //
    // wait for interrupt
    // writeBytes(0x03f020e3, 4, fptr);

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

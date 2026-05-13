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

void nuke(int mode, long offset) {
    FILE *fptr = fopen(OUTPUT, "wb");

    fwrite("PATCH", 1, 5, fptr);
    writeBytes(offset, 3, fptr);

    long bytes;
    size_t len;
    if (mode == 0) {        // branch
        len = 4;
        bytes = 0x1eff2fe1;
    } else if (mode == 1) { // loop
        len = 8;
        bytes = 0x00f020e3fdffffea;
    } else if (mode == 2) { // nop
        len = 4;
        bytes = 0x00f020e3;
    }

    writeBytes(len, 2, fptr);
    writeBytes(bytes, len, fptr);

    fwrite("EOF", 1, 3, fptr);

    fclose(fptr);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("ipbm [mode] [offset]\n  mode - [b]ranch, [l]oop or [n]op\n  offset - hex offset into code.bin\n");
        return 1;
    }

    char modeChar = argv[1][0];
    long offset = strtol(argv[2], NULL, 16);

    char modeName[10];
    int mode;
    if (modeChar == 'b') {
        strcpy(modeName, "branch");
        mode = 0;
    } else if (modeChar == 'l') {
        strcpy(modeName, "loop");
        mode = 1;
    } else if (modeChar == 'n') {
        strcpy(modeName, "nop");
        mode = 2;
    } else {
        printf("%c is not a valid mode", modeChar);
        return 1;
    }

    nuke(mode, offset);
    printf("made %s at code.bin offset 0x%lx\n", modeName, offset);
    printf("(remember: ghidra addresses are 0x00100000 too big!)\n");

    return 0;
}

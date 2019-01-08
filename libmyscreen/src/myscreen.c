#include <libmyscreen_private.h>

void myscreenClearScreen (void) {
    puts("\33[H\33[2J");
}

void myscreenGoto(int x,int y) {
    printf("\33[H\33[%d;%dH",y,x);
}

void myscreenWrite(int x,int y,char *c) {
    printf("\33[H\33[%d;%dH",y,x);
    printf(c);
}

void myscreenGotoTopLeft(void) {
    printf("\33[H");
}

void myscreenNextLine(void) {
    printf("\n");
}

void myscreenVideoInverse(void) {
    printf("\33[7m");
}

void myscreenBold(void) {
    printf("\33[1m");
}

void myscreenNorm(void) {
    printf("\33[0m");
}

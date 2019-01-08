#ifndef _LIBMYSCREEN_H_
#define _LIBMYSCREEN_H_

void myscreenClearScreen(void);
void myscreenGoto(int x,int y);
void myscreenWrite(int x,int y,char *c);
void myscreenGotoTopLeft(void);
void myscreenNextLine(void);
void myscreenVideoInverse(void);
void myscreenBold(void);
void myscreenNorm(void);

#endif

/* ROGUE 7/12/25 */

#include "rogue.h"

uint menu(char* c,uint os,char* o[]) {
    ROW=COL=0;
    INK=WHITE;
    ATR=BOLD;
    prints(c);
    ATR=NONE;
    for(int k=0;k<os;k++) {
        COL=2;
        ROW=k+2;
        prints("%c. %s",'a'+k,o[k]);
    }
    INK=YELLOW;
    ATR=BOLD;
    ROW+=2;
    COL=0;
    prints("Escoge una opcion (0 para abandonar el menu) ");
    INK=WHITE;
    ATR=NONE;
repregunta:
    listen(DELAY);
    if(inkey('0')) return os;
    for(char c='a';c<'a'+os;c++) {
        if(inkey(c) || inkey(c-'a'+'A')) return c-'a';
    }
    goto repregunta;
}

        


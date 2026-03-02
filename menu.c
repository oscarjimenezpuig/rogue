/* ROGUE 7/12/25 */

#include "rogue.h"

uint menu(char* c,uint os,char* o[]) {
    const int MAXBUF=4;
    ROW=COL=0;
    INK=WHITE;
    ATR=BOLD;
    prints(c);
    ATR=NONE;
    for(int k=0;k<os;k++) {
        COL=2;
        ROW=k+2;
        prints("%i. %s",k+1,o[k]);
    }
    INK=YELLOW;
    ATR=BOLD;
    ROW+=2;
    COL=0;
    prints("Escoge una opcion (0 para abandonar el menu) ");
    int select=-1;
    INK=WHITE;
    ATR=NONE;
    while(select<0 || select>=os) {
        listen(INPUT);
        char read[MAXBUF];
        bufget(MAXBUF-1,read);
        sscanf(read,"%i",&select);
    }
    if(select==0) return os;
    else return select-1;
}

        


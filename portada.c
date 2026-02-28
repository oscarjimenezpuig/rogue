/* ROGUE 28/2/26 */

#include "rogue.h"

#define WLEN 20 /* maxima longitud de las palabras */

#define RCENT ((ROWS-1)/2) /* da la fila centro de la pantalla */
#define CCENT(L) ((COLS-L)/2) /* da la columna centro en funcion de la longitud de la cadena */

static uint len(char* str) {
    /* da la longitud de la cadena */
    char* p=str;
    while(*p!=EOS) p++;
    return p-str;
}

static void writeword(int lw,char* w) {
    /* escribe una palabra si hay suficiente espacio */
    if(*w!=' ' || COL>0) {
        int esp=COLS-COL;
        if(lw>=esp) {
            COL=0;
            ROW++;
        }
        prints(w);
    }
}

static char* getword(uint* lw,char* w,char* str) {
    /* coge la siguiente palabra (separacion por espacios) */
    char* p=str;
    *lw=0;
    while(*p==' ') p++;
    if(*p!=EOS) {
        char* pw=w;
        while(*p!=' ' && *p!=EOS) {
            *pw++=*p++;
        }
        *pw=EOS;
        *lw=pw-w;
        return p;
    }
    return NULL;
}

static void write(char* str) {
    char* ps=str;
    char word[WLEN];
    uint len=0;
    while((ps=getword(&len,word,ps))) {
        writeword(len,word);
        writeword(1," ");
    }
}

static void atr(uint i,uint p,uint a) {
    /* establece los atributos */
    INK=i;
    BKG=p;
    ATR=a;
};

static void at(uint r,uint c) {
    ROW=r;
    COL=c;
}

void portada() {
    const int INI=3;
    char* titulo="EL ANILLO UNICO";
    char* autor="Oscar Jimenez Puig (c)2026";
    char* explc="Cuando parecia que FRODO habia destruido el anillo unico en el MONTE DEL DESTINO, SAURON lo recupero y en un viaje multidimensional, se escondio en las maxmorras de ZOR-EKEL junto a DRAKON. Tu objetivo es, de una vez por todas, recuperar el anillo unico y huir de las mazmorras.";
    char* apyo="Pulsa la tecla AYUDA (H) para conocer las ordenes disponibles.";
    char* msch="Mucha suerte en tu casi imposible mision...";
    cls();
    at(INI,CCENT(len(titulo)));
    atr(BLACK,YELLOW,NONE);
    prints(titulo);
    at(INI+3,CCENT(len(autor)));
    atr(WHITE,BLACK,BOLD);
    prints(autor);
    at(INI+7,0);
    atr(WHITE,BLACK,NONE);
    write(explc);
    at(ROW+2,0);
    write(apyo);
    show();
    pause(2);
    atr(RED,BLACK,BOLD);
    at(ROW+2,0);
    write(msch);
    show();
    pause(1);
    at(ROWS-1,0);
    atr(WHITE,BLACK,REVERSE|BLINK);
    prints("Pulsa S para empezar");
    show();
    do {
        listen(DELAY);
    } while(inkey('S')==0);
    atr(WHITE,BLACK,NONE);
    cls();
}

static void tutoblack() {
    atr(BLACK,BLACK,NONE);
    for(int c=0;c<COLS;c++) {
        for(int r=0;r<ROWS;r++) {
            at(r,c);
            prints(" ");
            show();
            pause(0.01);
        }
    }
}

void final_perder() {
    char* frase="HAS MUERTO... ";
    tutoblack();
    for(int ink=WHITE;ink>=BLACK;ink--) {
        at(RCENT,CCENT(len(frase)));
        atr(ink,BLACK,BOLD);
        prints(frase);
        show();
        pause(1);
    }
    pause(2);
    cls();
}

static void tutocolo() {
    for(int r=0;r<ROWS;r++) {
        for(int c=0;c<COLS;c++) {
            at(r,c);
            atr(rnd(BLACK,WHITE),BLACK,BOLD);
            prints(" ");
            show();
            pause(0.01);
        }
    }
}

void final_ganar() {
    char* frase="LO HAS CONSEGUIDO";
    tutoblack();
    for(int ink=WHITE;ink>=BLACK;ink--) {
        at(RCENT,CCENT(len(frase)));
        atr(ink,BLACK,BOLD|BLINK);
        prints(frase);
        show();
        pause(2);
    }
    tutocolo();
    tutoblack();
    cls();
}
    






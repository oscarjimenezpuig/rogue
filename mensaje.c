/*
============================================================
  Fichero: mensaje.c
  Creado: 06-12-2025
  Ultima Modificacion: dimecres, 17 de desembre de 2025, 18:27:07
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

/* mensaje */
#define STRCON "MAS" /* string que se pone al final del mensaje si tiene continuacion */


void mensaje(char* m) {
	char* pm=m;
	INK=WHITE;
	ATR=NONE;
	while(*pm!=EOS) {
		int counter=0;
		ROW=COL=0;
		while((counter<COLS-10 || *pm!=' ') && *pm!=EOS) {
			printc(*pm++);
		}
		if(*pm!=EOS) {
			ATR=REVERSE;
			prints("   %s",STRCON);
		}
		while(listen(INKEY)==0);
		ATR=NONE;
		ROW=COL=0;
		for(int c=0;c<COLS;c++) {
			printc(' ');
		}
	}
}

#undef EOS

		

	

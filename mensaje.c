/*
============================================================
  Fichero: mensaje.c
  Creado: 06-12-2025
  Ultima Modificacion: vie 19 dic 2025 08:49:33
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdarg.h>

#include "rogue.h"

/* mensaje */
#define MENLEN 1024 /* longitud del mensaje */
#define STRCON "MAS" /* string que se pone al final del mensaje si tiene continuacion */


void mensaje(const char* im,...) {
	char m[MENLEN];
	va_list list;
	va_start(list,im);
	vsnprintf(m,MENLEN-1,im,list);
	va_end(list);
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

		

	

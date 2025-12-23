/*
============================================================
  Fichero: mensaje.c
  Creado: 06-12-2025
  Ultima Modificacion: dimarts, 23 de desembre de 2025, 18:13:37
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdarg.h>

#include "rogue.h"

/* constantes */
#define MENLEN 1024 /* longitud de la lista mensaje */

/* variables */
static char mensaje[MENLEN]={'\0'};
char* pmensaje=mensaje;

void menin(const char* im,...) {
	uint len=MENLEN-2-(pmensaje-mensaje);
	char m[len];
	va_list list;
	va_start(list,im);
	vsnprintf(m,len-1,im,list);
	va_end(list);
	char* pm=m;
	while(*pm!=EOS) {
		*pmensaje++=*pm++;
	}
	*pmensaje++=EOS;
	*pmensaje=EOS;
}

static Bool onemenout() {
	if(*pmensaje!=EOS) {
		while(*pmensaje!=EOS) {
			ROW=COL=0;
			while((COL<COLS-10 || *pmensaje!=' ') && *pmensaje!=EOS) {
				printc(*pmensaje++);
			}
			if(*pmensaje!=EOS) {
				ATR=REVERSE;
				COL=COL+3;
				prints("MAS");
			}
			while(listen(INKEY)==0);
			ATR=NONE;
			ROW=COL=0;
			for(;COL<COLS;) printc(' ');
		}
		pmensaje++;
		return TRUE;
	} else return FALSE;
}

void menout() {
	ROW=COL=0;
	INK=WHITE;
	BKG=BLACK;
	ATR=NONE;
	pmensaje=mensaje;
	while(onemenout());
	pmensaje=mensaje;
	*pmensaje=EOS;
}

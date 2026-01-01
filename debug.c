/*
============================================================
  Fichero: debug.c
  Creado: 01-01-2026
  Ultima Modificacion: dijous, 1 de gener de 2026, 10:16:42
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include <stdarg.h>
#include <stdlib.h>

#include "rogue.h"

static int deb_row=0;

void dbgprt(const char* s,...) {
	ROW=deb_row++;
	COL=0;
	INK=RED;
	BKG=BLACK;
	prints("Debug: ");
	va_list list;
	va_start(list,s);
	char str[ROWS];
	vsprintf(str,s,list);
	va_end(list);
	INK=WHITE;
	prints(str);
	while(listen(INKEY)==0);
	if(inkey('Q')) exit(0);
}

void dbgclr() {
	cls();
	deb_row=0;
}


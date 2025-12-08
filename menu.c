/*
============================================================
  Fichero: menu.c
  Creado: 07-12-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 08:39:03
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

u1 menu(char* c,u1 os,char* o[]) {
	u1 select=0;
	inmode(DELAY);
presenta:
	cls();
	at(0,0);
	ink(WHITE);
	attr(BOLD);
	prints(c);
	attr(0);
	for(int k=0;k<os;k++) {
		at(k,2);
		if(k==select) attr(REVERSE);
		else attr(0);
		prints("%i. %s",k+1,o[k]);
	}
teclado:
	listen();
	if(inkey(TARR)) {
		if(select>0) --select;
		else select=os-1;
		goto presenta;
	} else if(inkey(TABJ)) {
		if(select==os-1) select=0;
		else ++select;
		goto presenta;
	} else if(inkey(TMFI)) {
		return os;
	} else if(inkey(TMOK)) {
		return select;
	} else {
		goto teclado;
	}
}
	




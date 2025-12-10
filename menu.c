/*
============================================================
  Fichero: menu.c
  Creado: 07-12-2025
  Ultima Modificacion: mié 10 dic 2025 11:00:46
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

/* teclado de menu */
#define TMSU 'i' /* tecla de sube en menu */
#define TMBA 'k' /* tecla de baja en menu */
#define TMFI 'M' /* cerrar menu sin seleccionar */
#define TMOK 'm' /* cerrar menu seleccionando */

uint menu(char* c,uint os,char* o[]) {
	uint select=0;
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
	if(inkey(TMSU)) {
		if(select>0) --select;
		else select=os-1;
		goto presenta;
	} else if(inkey(TMBA)) {
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
	




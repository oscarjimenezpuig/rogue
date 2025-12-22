/*
============================================================
  Fichero: menu.c
  Creado: 07-12-2025
  Ultima Modificacion: dilluns, 22 de desembre de 2025, 22:58:53
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
presenta:
	ROW=COL=0;
	INK=WHITE;
	ATR=BOLD;
	prints(c);
	ATR=NONE;
	for(COL=0;COL<os;COL++) {
		ROW=2;
		if(COL==select) ATR=REVERSE;
		prints("%i. %s",COL+1,o[COL]);
		ATR=NONE;
	}
teclado:
	listen(DELAY);
	if(inkey(TMSU)) {
		if(select>0) --select;
		else select=os-1;
		goto presenta;
	} else if(inkey(TMBA)) {
		if(select==os-1) select=0;
		else ++select;
		goto presenta;
	} else if(inkey(TMFI)) {
		cls();
		return os;
	} else if(inkey(TMOK)) {
		cls();
		return select;
	} else {
		goto teclado;
	}
}
	




/* ROGUE 7/12/25 */

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
	for(int k=0;k<os;k++) {
		COL=2;
		ROW=k+2;
		if(ROW==select+2) ATR=REVERSE;
		prints("%i. %s",k+1,o[k]);
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
		return os;
	} else if(inkey(TMOK)) {
		return select;
	} else {
		goto teclado;
	}
}
	




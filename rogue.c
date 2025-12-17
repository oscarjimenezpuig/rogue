/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: dimecres, 17 de desembre de 2025, 18:57:42
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void curse() {
	mapnew(FALSE,TRUE);
	jugnew();
	cls();
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: jue 18 dic 2025 13:13:53
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

uint nivel=1;

void curse() {
	randomize(-1);
	mapnew(FALSE,TRUE);
	jugnew();
	llplev();
	cls();
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



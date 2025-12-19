/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: vie 19 dic 2025 11:58:32
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
	orolev();
	cls();
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



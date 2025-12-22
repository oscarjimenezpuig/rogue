/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: dilluns, 22 de desembre de 2025, 21:43:29
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
	rhrlev();
	cls();
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: vie 12 dic 2025 13:14:31
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"


void begin() {
	mapnew(FALSE,TRUE);
	jugnew();
	cls();
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



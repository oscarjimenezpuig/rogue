/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: dijous, 11 de desembre de 2025, 04:36:28
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"


void begin() {
	mapnew(FALSE,TRUE);
	jugnew();
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



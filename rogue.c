/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: dijous, 25 de desembre de 2025, 09:13:18
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void curse() {
	nivprm();	
	while(jugador) {
		if(jugshw()) {
			jugact();
		} else break;
	}
}



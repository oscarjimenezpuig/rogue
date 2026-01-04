/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: diumenge, 4 de gener de 2026, 08:48:56
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void curse() {
	nivprm();	
	while(jugador) {
		if(jugshw()) {
			jugact();
			eneact();
		} else break;
	}
}



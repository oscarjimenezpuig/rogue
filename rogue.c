/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: mié 21 ene 2026 12:21:58
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void curse() {
	nivprm();	
	while(jugador) {
		if(jugshw()) {
			if(jugact()) {
				eneact(); //el enemigo solo actua si la orden del jugador es correcta
				fanact();
			}
		} else break;
	}
}



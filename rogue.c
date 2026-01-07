/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: dimecres, 7 de gener de 2026, 11:58:18
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void curse() {
	nivprm();	
	while(jugador) {
		if(jugshw()) {
			if(jugact()) eneact(); //el enemigo solo actua si la orden del jugador es correcta
		} else break;
	}
}



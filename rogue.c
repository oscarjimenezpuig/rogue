/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: mié 10 dic 2025 15:20:33
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void begin() {
	mapnew(FALSE,TRUE);
	jugnew();
	while(!quit) {
		jugshw();
		jugact();
	}
}



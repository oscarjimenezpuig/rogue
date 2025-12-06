/*
============================================================
  Fichero: rogue.c
  Creado: 05-12-2025
  Ultima Modificacion: dissabte, 6 de desembre de 2025, 12:24:23
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

static void rogueshow() {
	int dtr,dtc;
	dimget(&dtr,&dtc);
	int jr=PJUG->r;
	int jc=PJUG->c;
	int mrs=dtr-3;
	int mcs=dtc;
	int mri=jr-mrs/2;
	int mci=jc-mcs/2;
	int ro=1;
	int co=0;
	showscr(mri,mci,mrs,mcs,ro,co); //mostramos pantalla
}

void begin() {
	mapnew();
	if(jugnew()) {
		while(inkey('q')==0) {
			rogueshow();
			jugact();
		}
	}
}



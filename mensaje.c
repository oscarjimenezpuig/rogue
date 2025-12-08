/*
============================================================
  Fichero: mensaje.c
  Creado: 06-12-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 08:27:25
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

#define EOS '\0'

void mensaje(char* m) {
	int rs,cs;
	dimget(&rs,&cs);
	char* pm=m;
	ink(7);
	attr(0);
	while(*pm!=EOS) {
		int counter=0;
		at(0,0);
		while((counter<cs-10 || *pm!=' ') && *pm!=EOS) {
			printc(*pm++);
		}
		if(*pm!=EOS) {
			attr(REVERSE);
			prints("   %s",STRCON);
		}
		while(listen()==0);
		attr(0);
		at(0,0);
		for(int c=0;c<cs;c++) {
			printc(' ');
		}
	}
}

#undef EOS

		

	

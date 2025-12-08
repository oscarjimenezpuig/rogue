/*
============================================================
  Fichero: oro.c
  Creado: 08-12-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 20:03:53
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

void orosnew(int tot) {
	while(tot>0) {
		int co=rnd(ORCPNI/4,ORCPNI/2);
		co=(co>tot)?tot:co;
		tot-=co;
		objeto_t no=objsfree();
		item_t itor;
		itor.contenedor=OBJNUL;
		itor.oro=co;
		if(itmnew(no,"Oro",COGIBLE|ORO,ATRORO,itor)) {
			int r,c;
			maprndpos(&r,&c,HABITACION);
			pobjeto_t poro=objget(no);
			poro->r=r;
			poro->c=c;
		}
	}
}
	

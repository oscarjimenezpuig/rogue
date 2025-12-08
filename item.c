/*
============================================================
  Fichero: item.c
  Creado: 07-12-2025
  Ultima Modificacion: diumenge, 7 de desembre de 2025, 19:50:23
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

u1 itmnew(objeto_t o,char* n,u2 t,atributo_t a,item_t i) {
	if(objnew(o,t)) {
		objname(o,n);
		pobjeto_t po=objget(o);
		po->atributo=a;
		po->item=i;
		return 1;
	}
	return 0;
}



/*
============================================================
  Fichero: objeto.c
  Creado: 30-11-2025
  Ultima Modificacion: dilluns, 1 de desembre de 2025, 18:56:23
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

struct objeto_s objetos[OBJETOS];

static void objini() {
	static u1 inited=0;
	if(!inited) {
		pobjeto p=objetos;
		while(p!=objetos+OBJETOS) p++->type=0;
		inited=1;
	}
}

u1 objnew(u2 id,u1 t) {
	objini();
	if(id<OBJETOS) {
		pobjeto=objetos+id;
		pobjeto->type=t;
		return 1;
	}
	return 0;
}

pobjeto objget(u1 id) {
	objini();
	return (id<OBJETOS && (objetos+id)->type!=0)?objetos+id:NULL;
}


		
	

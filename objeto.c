/*
============================================================
  Fichero: objeto.c
  Creado: 30-11-2025
  Ultima Modificacion: dilluns, 8 de desembre de 2025, 19:28:02
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

struct objeto_s objetos[OBJETOS];

static void objini() {
	static u1 inited=0;
	if(!inited) {
		pobjeto_t p=objetos;
		while(p!=objetos+OBJETOS) p++->tipo=0;
		inited=1;
	}
}

u1 objnew(objeto_t o,u2 t) {
	objini();
	if(o<OBJETOS) {
		pobjeto_t po=objetos+o;
		po->tipo=t;
		return 1;
	}
	return 0;
}

void objname(objeto_t o,char* n) {
	pobjeto_t po=objget(o);
	if(po) {
		char* pn=n;
		char* pon=po->nombre;
		while(*pn!='\0' && pn-n<=NAMOBJLEN) {
			*pon++=*pn++;
		}
		*pon='\0';
	}
}

pobjeto_t objget(objeto_t o) {
	objini();
	return (o<OBJETOS && (objetos+o)->tipo!=0)?objetos+o:NULL;
}

u2 objsfnd(objeto_t* os,Condicion c) {
	objeto_t *pos=os;
	pobjeto_t ptro=objetos;
	while(ptro!=objetos+OBJETOS) {
		if(c(ptro-objetos)) {
			*pos++=(ptro-objetos);
		}
		ptro++;
	}
	return pos-os;
}

u2 objsfree() {
	pobjeto_t p=objetos;
	while(p!=objetos+OBJETOS) {
		if(p->tipo==0) return p-objetos;
		p++;
	}
	return OBJNUL;
}





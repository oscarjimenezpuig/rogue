/*
============================================================
  Fichero: rogue.c
  Creado: 10-12-2025
  Ultima Modificacion: jue 22 ene 2026 12:24:18
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

static Bool isnpc(objeto_t* o) {
	return (o && o->npc && o->jug==0 && objcanact(o));
}

static void npcsact() {
	static uint npcs=0;
	jugact();
	jugshw(jugador);
	objeto_t* npc[objsiz()];
	npcs=objfnd(npc,isnpc);
	if(npcs) {
		objeto_t** p=npc;
		while(p!=npc+npcs) {
			if((*p)->jug==0 && (*p)->fan==0) eneact(*p);
			else fanact();
			jugshw(*p);
			p++;
		}
	}
}

void curse() {
	nivprm();
	static Bool first=TRUE;
	while(jugador) {
		if(first) {
			jugshw(jugador);
			first=FALSE;
		}
		npcsact(); /* actuan todos los npcs */
		fanset(); /* conexion de fantasma si no esta conectado */
	}
}



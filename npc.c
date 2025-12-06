/*
============================================================
  Fichero: npc.c
  Creado: 05-12-2025
  Ultima Modificacion: dissabte, 6 de desembre de 2025, 09:35:03
  oSCAR jIMENEZ pUIG                                       
============================================================
*/

#include "rogue.h"

u1 npcnew(objeto_t o,char* n,atributo_t a,u1 mov,npc_t npc) {
	u1 tipo=NPC|((mov)?MOVIL:0);
	if(objnew(o,tipo)) {
		pobjeto_t po=objget(o);
		char* pn=n;
		char* pnom=po->nombre;
		while(*pn!='\0' && pnom-po->nombre<NAMOBJLEN) {
			*pnom++=*pn++;
		}
		*pnom='\0';
		po->atributo=a;
		po->npc=npc;
		po->npc.vact=po->npc.velocidad;
		return 1;	
	}
	return 0;
}

static u1 isnpc(objeto_t o) {
	pobjeto_t po=objget(o);
	return (po && (po->tipo & NPC));
}

u2 npcfnd(pobjeto_t* pn) {
	objeto_t n[OBJETOS];
	u2 ns=objsfnd(n,isnpc);
	for(u2 k=0;k<ns;k++) {
		pn[k]=objget(n[k]);
	}
	return ns;
}

u1 npcinplace(int r,int c) {
	pobjeto_t po[OBJETOS];
	u2 pos=npcfnd(po);
	for(u2 k=0;k<pos;k++) {
		if(po[k]->r==r && po[k]->c==c) return 1;
	}
	return 0;
}		

u1 npccanpos(objeto_t o,int r,int c) {
	if((mapget(r,c)->tipo)!=TRANSITABLE) return 0;
	else if(npcinplace(r,c)) return 0;
	else return 1;
}

u1 npcpos(objeto_t o,int r,int c) {
	if(npccanpos(o,r,c)) {
		pobjeto_t po=objget(o);
		po->r=r;
		po->c=c;
		return 1;
	}
	return 0;
}

u1 npcrndpos(objeto_t o,u1 tt) {
	const u2 TRIES=MAPAA;
	u2 tries=TRIES;
	int r,c;
	while(tries--) {
		if(maprndpos(&r,&c,tt)) {
			if(!npcinplace(r,c)) {
				pobjeto_t po=objget(o);
				po->r=r;
				po->c=c;
				return 1;
			}
		}
	}
	return 0;
}

u1 npccanact(objeto_t o) {
	pobjeto_t po=objget(o);
	if(po && po->tipo & NPC) {
		if(po->npc.vact==po->npc.velocidad) {
			po->npc.vact=0;
			return 1;
		} else {
			po->npc.vact++;
		}
	}
	return 0;
}
	

